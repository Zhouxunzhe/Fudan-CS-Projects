#include "b_tree.h"
#include "buffer_pool.h"

#include <stdio.h>

void b_tree_init(const char* filename, BufferPool* pool) {
    init_buffer_pool(filename, pool);
    /* TODO: add code here */
    if (pool->file.length == 0) {
        for (int i = 0; i < 2; i++) {
            Page* init = get_page(pool, i * PAGE_SIZE);
            release(pool, i * PAGE_SIZE);
        }
        //BCtrlBlock initialed
        BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
        ctrl->root_node = PAGE_SIZE;
        ctrl->free_node_head = -1;
        ctrl->n_node = 2;
        release(pool, 0);

        //root initialed
        BNode* root = (BNode*)get_page(pool, PAGE_SIZE);
        root->n = 0;
        root->next = -1;
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            root->child[i] = -1;
        }
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            get_rid_block_addr(root->row_ptr[i]) = -1;
            get_rid_idx(root->row_ptr[i]) = 0;
        }
        release(pool, PAGE_SIZE);
    }
    return;
}

static off_t FindMostLeft(BufferPool* pool, off_t addr) {
    off_t tmp_addr = addr;

    BNode* tmp_node = (BNode*)get_page(pool, tmp_addr);
    release(pool, tmp_addr);

    while (tmp_addr != -1 && tmp_node->child[0] != -1) {
        tmp_node = (BNode*)get_page(pool, tmp_addr);
        release(pool, tmp_addr);
        tmp_addr = tmp_node->child[0];
    }
    return tmp_addr;
}

static off_t FindMostRight(BufferPool* pool, off_t addr) {
    off_t tmp_addr = addr;

    BNode* tmp_node = (BNode*)get_page(pool, tmp_addr);
    release(pool, tmp_addr);

    while (tmp_addr != -1 && tmp_node->child[tmp_node->n - 1] != -1) {
        tmp_node = (BNode*)get_page(pool, tmp_addr);
        release(pool, tmp_addr);
        tmp_addr = tmp_node->child[tmp_node->n - 1];
    }
    return tmp_addr;
}

static off_t FindSibling(BufferPool* pool, off_t par_addr, int dst) {
    off_t sib_addr = -1;
    int Limit;
    Limit = 2 * DEGREE;
    BNode* Parent, * child_1 = NULL, * child_dst_sub1 = NULL, * child_dst_add1 = NULL;

    Parent = (BNode*)get_page(pool, par_addr);

    if (Parent->child[1] != -1) {
        child_1 = (BNode*)get_page(pool, Parent->child[1]);
    }
    if (Parent->child[dst - 1] != -1) {
        child_dst_sub1 = (BNode*)get_page(pool, Parent->child[dst - 1]);
    }
    if (Parent->child[dst + 1] != -1) {
        child_dst_add1 = (BNode*)get_page(pool, Parent->child[dst + 1]);
    }
    release(pool, Parent->child[1]);
    release(pool, Parent->child[dst - 1]);
    release(pool, Parent->child[dst + 1]);

    if (dst == 0) {
        if (child_1->n < Limit && Parent->child[1] != -1)
            sib_addr = Parent->child[1];
    }
    else if (child_dst_sub1->n < Limit && Parent->child[dst - 1] != -1) {
        sib_addr = Parent->child[dst - 1];
    }
    else if (dst + 1 < Parent->n && child_dst_add1->n < Limit && Parent->child[dst + 1] != -1) {
        sib_addr = Parent->child[dst + 1];
    }
    release(pool, par_addr);

    return sib_addr;
}

static off_t FindSiblingKeyNum_M_2(BufferPool* pool, off_t par_addr, int dst, int* loc) {
    int Limit;
    off_t sib_addr = -1;
    Limit = LIMIT_DEGREE_2;
    BNode* Parent, * child_1 = NULL, * child_dst_sub1 = NULL, * child_dst_add1 = NULL;

    Parent = (BNode*)get_page(pool, par_addr);

    if (Parent->child[1] != -1) {
        child_1 = (BNode*)get_page(pool, Parent->child[1]);
    }
    if (Parent->child[dst - 1] != -1) {
        child_dst_sub1 = (BNode*)get_page(pool, Parent->child[dst - 1]);
    }
    if (Parent->child[dst + 1] != -1) {
        child_dst_add1 = (BNode*)get_page(pool, Parent->child[dst + 1]);
    }
    release(pool, Parent->child[1]);
    release(pool, Parent->child[dst - 1]);
    release(pool, Parent->child[dst + 1]);

    if (dst == 0) {
        if (child_1->n > Limit && Parent->child[1] != -1) {
            sib_addr = Parent->child[1];
            *loc = 1;
        }
    }
    else {
        if (child_dst_sub1->n > Limit && Parent->child[dst - 1] != -1) {
            sib_addr = Parent->child[dst - 1];
            *loc = dst - 1;
        }
        else if (dst + 1 < Parent->n && child_dst_add1->n > Limit && Parent->child[dst + 1] != -1) {
            sib_addr = Parent->child[dst + 1];
            *loc = dst + 1;
        }
    }
    release(pool, par_addr);

    return sib_addr;
}

off_t InsertElement(BufferPool* pool, int isKey, off_t par_addr, off_t target_addr, RID rid, int loc, int dst) {
    int k;
    BNode* target_node = (BNode*)get_page(pool, target_addr);
    BNode* Parent = (BNode*)get_page(pool, par_addr);

    if (isKey) {//insert rid
        k = target_node->n - 1;
        while (k >= dst) {
            target_node->row_ptr[k + 1] = target_node->row_ptr[k]; k--;
        }

        target_node->row_ptr[dst] = rid;

        if (par_addr != -1) {//not root, move to parent node
            Parent->row_ptr[loc] = target_node->row_ptr[0];
        }
        target_node->n++;
    }
    else {//insert node
        if (target_node->child[0] == -1) {//leaf node, joint nodes
            if (loc > 0 && Parent->child[loc - 1] != -1) {
                BNode* child_loc_sub1 = (BNode*)get_page(pool, Parent->child[loc - 1]);
                release(pool, Parent->child[loc - 1]);
                child_loc_sub1->next = target_addr;
            }
            target_node->next = Parent->child[loc];
        }

        k = Parent->n - 1;
        while (k >= loc) {
            Parent->child[k + 1] = Parent->child[k];
            Parent->row_ptr[k + 1] = Parent->row_ptr[k];
            k--;
        }
        Parent->row_ptr[loc] = target_node->row_ptr[0];
        Parent->child[loc] = target_addr;

        Parent->n++;
    }
    release(pool, target_addr);
    release(pool, par_addr);
    return target_addr;
}

static off_t RemoveElement(BufferPool* pool, int isKey, off_t par_addr, off_t target_addr, int dst, int loc) {
    RID Unavailable;
    get_rid_block_addr(Unavailable) = -1;
    get_rid_idx(Unavailable) = 0;
    int k, Limit;

    BNode* target_node = (BNode*)get_page(pool, target_addr);
    BNode* Parent = (BNode*)get_page(pool, par_addr);

    if (isKey) {//delete key
        Limit = target_node->n;
        k = loc + 1;
        while (k < Limit) {
            target_node->row_ptr[k - 1] = target_node->row_ptr[k]; k++;
        }
        target_node->row_ptr[target_node->n - 1] = Unavailable;
        Parent->row_ptr[dst] = target_node->row_ptr[0];
        target_node->n--;
    }
    else {//delete node
        if (target_node->child[0] == -1 && dst > 0 && Parent->child[dst - 1] != -1) {
            BNode* child_dst_sub1 = (BNode*)get_page(pool, Parent->child[dst - 1]);
            child_dst_sub1->next = Parent->child[dst - 1];
            release(pool, Parent->child[dst - 1]);
        }
        Limit = Parent->n;
        k = dst + 1;
        while (k < Limit) {
            Parent->child[k - 1] = Parent->child[k];
            Parent->row_ptr[k - 1] = Parent->row_ptr[k];
            k++;
        }

        Parent->child[Parent->n - 1] = -1;
        Parent->row_ptr[Parent->n - 1] = Unavailable;
        Parent->n--;
    }
    release(pool, par_addr);
    release(pool, target_addr);
    return target_addr;
}

off_t MoveElement(BufferPool* pool, off_t src_addr, off_t dst_addr, off_t par_addr, int i, int n, b_tree_row_row_cmp_t cmp) {
    RID Unavailable;
    get_rid_block_addr(Unavailable) = -1;
    get_rid_idx(Unavailable) = 0;
    RID TmpKey;
    off_t child_addr, mr_addr, ml_addr;
    int j = 0, SrcInFront = 0;

    BNode* Parent, * mr;
    BNode* src_node = (BNode*)get_page(pool, src_addr);
    BNode* dst_node = (BNode*)get_page(pool, dst_addr);
    release(pool, dst_addr);
    release(pool, src_addr);

    if (cmp(src_node->row_ptr[0], dst_node->row_ptr[0]) < 0)
        SrcInFront = 1;

    if (SrcInFront) {
        if (src_node->child[0] != -1) {
            while (j < n) {
                src_node = (BNode*)get_page(pool, src_addr);
                release(pool, src_addr);
                child_addr = src_node->child[src_node->n - 1];
                RemoveElement(pool, 0, src_addr, child_addr, src_node->n - 1, -1);
                InsertElement(pool, 0, dst_addr, child_addr, Unavailable, 0, -1);
                j++;
            }
        }
        else {
            while (j < n) {
                src_node = (BNode*)get_page(pool, src_addr);
                release(pool, src_addr);
                TmpKey = src_node->row_ptr[src_node->n - 1];
                RemoveElement(pool, 1, par_addr, src_addr, i, src_node->n - 1);
                InsertElement(pool, 1, par_addr, dst_addr, TmpKey, i + 1, 0);
                j++;
            }

        }

        Parent = (BNode*)get_page(pool, par_addr);
        dst_node = (BNode*)get_page(pool, dst_addr);
        src_node = (BNode*)get_page(pool, src_addr);
        Parent->row_ptr[i + 1] = dst_node->row_ptr[0];
        release(pool, src_addr);
        release(pool, dst_addr);
        release(pool, par_addr);
        //rejoint leaf nodes
        if (src_node->n > 0) {
            mr_addr = FindMostRight(pool, src_addr);
            ml_addr = FindMostLeft(pool, dst_addr);
            mr = (BNode*)get_page(pool, mr_addr);
            mr->next = ml_addr;
            release(pool, mr_addr);
        }
    }
    else {
        if (src_node->child[0] != -1) {
            while (j < n) {
                src_node = (BNode*)get_page(pool, src_addr);
                dst_node = (BNode*)get_page(pool, dst_addr);
                release(pool, dst_addr);
                release(pool, src_addr);
                child_addr = src_node->child[0];
                RemoveElement(pool, 0, src_addr, child_addr, 0, -1); 
                src_node = (BNode*)get_page(pool, src_addr);
                dst_node = (BNode*)get_page(pool, dst_addr);
                release(pool, dst_addr);
                release(pool, src_addr);
                InsertElement(pool, 0, dst_addr, child_addr, Unavailable, dst_node->n, -1);
                j++;
            }

        }
        else {
            while (j < n) {
                src_node = (BNode*)get_page(pool, src_addr);
                dst_node = (BNode*)get_page(pool, dst_addr);
                release(pool, dst_addr);
                release(pool, src_addr);
                TmpKey = src_node->row_ptr[0];
                off_t addr = get_rid_block_addr(TmpKey);
                off_t idx = get_rid_idx(TmpKey);
                RemoveElement(pool, 1, par_addr, src_addr, i, 0);
                src_node = (BNode*)get_page(pool, src_addr);
                dst_node = (BNode*)get_page(pool, dst_addr);
                release(pool, dst_addr);
                release(pool, src_addr);
                InsertElement(pool, 1, par_addr, dst_addr, TmpKey, i - 1, dst_node->n);
                j++;
            }
        }
        Parent = (BNode*)get_page(pool, par_addr);
        src_node = (BNode*)get_page(pool, src_addr);
        Parent->row_ptr[i] = src_node->row_ptr[0];
        release(pool, src_addr);
        release(pool, par_addr);
        if (src_node->n > 0) {
            mr_addr = FindMostRight(pool, src_addr);
            ml_addr = FindMostLeft(pool, dst_addr);
            mr = (BNode*)get_page(pool, mr_addr);
            mr->next = ml_addr;
            release(pool, mr_addr);
        }
    }

    return par_addr;
}

off_t SplitNode(BufferPool* pool, off_t par_addr, off_t target_addr, int i) {
    RID Unavailable;
    get_rid_block_addr(Unavailable) = -1;
    get_rid_idx(Unavailable) = 0;
    int j, k, Limit;
    BNode* NewNode, * freenode, * target_node, * Parent;
    off_t newnode_addr;

    BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
    if (ctrl->free_node_head != -1) {//use freenode
        newnode_addr = ctrl->free_node_head;
        freenode = (BNode*)get_page(pool, ctrl->free_node_head);
        release(pool, ctrl->free_node_head);
        ctrl->free_node_head = freenode->next;
    }
    else {
        newnode_addr = PAGE_SIZE * ctrl->n_node;
        ctrl->n_node++;
    }
    release(pool, 0);

    NewNode = (BNode*)get_page(pool, newnode_addr);
    NewNode->n = 0;
    NewNode->next = -1;
    for (int i = 0; i < 2 * DEGREE + 1; i++) {
        NewNode->child[i] = -1;
    }
    for (int i = 0; i < 2 * DEGREE + 1; i++) {
        get_rid_block_addr(NewNode->row_ptr[i]) = -1;
        get_rid_idx(NewNode->row_ptr[i]) = 0;
    }
    target_node = (BNode*)get_page(pool, target_addr);

    k = 0;
    j = target_node->n / 2;
    Limit = target_node->n;
    while (j < Limit) {
        if (target_node->child[0] != -1) {//not leaf, move from target to newnode
            NewNode->child[k] = target_node->child[j];
            target_node->child[j] = -1;
        }
        NewNode->row_ptr[k] = target_node->row_ptr[j];
        target_node->row_ptr[j] = Unavailable;
        NewNode->n++; target_node->n--;
        j++; k++;
    }
    release(pool, newnode_addr);
    release(pool, target_addr);

    if (par_addr != -1)
        InsertElement(pool, 0, par_addr, newnode_addr, Unavailable, i + 1, -1);
    else {
        //if root,create new root
        ctrl = (BCtrlBlock*)get_page(pool, 0);
        if (ctrl->free_node_head != -1) {//use freenode
            par_addr = ctrl->free_node_head;
            freenode = (BNode*)get_page(pool, ctrl->free_node_head);
            release(pool, ctrl->free_node_head);
            ctrl->free_node_head = freenode->next;
        }
        else {
            par_addr = PAGE_SIZE * ctrl->n_node;
            ctrl->n_node++;
        }
        ctrl->root_node = par_addr;
        release(pool, 0);

        Parent = (BNode*)get_page(pool, par_addr);
        Parent->n = 0;
        Parent->next = -1;
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            Parent->child[i] = -1;
        }
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            get_rid_block_addr(Parent->row_ptr[i]) = -1;
            get_rid_idx(Parent->row_ptr[i]) = 0;
        }
        release(pool, par_addr);

        InsertElement(pool, 0, par_addr, target_addr, Unavailable, 0, -1);
        InsertElement(pool, 0, par_addr, newnode_addr, Unavailable, 1, -1);

        return par_addr;
    }
    return target_addr;
}

static off_t MergeNode(BufferPool* pool, off_t par_addr, off_t target_addr, off_t neighbor_addr, int i, b_tree_row_row_cmp_t cmp) {
    int Limit;

    BNode* neighbor = (BNode*)get_page(pool, neighbor_addr);
    BNode* target_node = (BNode*)get_page(pool, target_addr);
    release(pool, target_addr);
    release(pool, neighbor_addr);
    if (neighbor->n > LIMIT_DEGREE_2) {
        //merge an element from neighbor to target
        MoveElement(pool, neighbor_addr, target_addr, par_addr, i, 1, cmp);
    }
    else {//merge all element from target to neighbor,delete target
        Limit = target_node->n;
        MoveElement(pool, target_addr, neighbor_addr, par_addr, i, Limit, cmp);
        RemoveElement(pool, 0, par_addr, target_addr, i, -1);
        //free(target_node);
        BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
        target_node = (BNode*)get_page(pool, target_addr);
        target_node->next = ctrl->free_node_head;
        ctrl->free_node_head = target_addr;
        //clear data
        target_node->n = 0;
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            target_node->child[i] = -1;
        }
        for (int i = 0; i < 2 * DEGREE + 1; i++) {
            get_rid_block_addr(target_node->row_ptr[i]) = -1;
            get_rid_idx(target_node->row_ptr[i]) = 0;
        }
        release(pool, target_addr);
        release(pool, 0);
    }

    return par_addr;
}

static off_t insert(BufferPool* pool, off_t root_addr, RID rid, int i, off_t par_addr, b_tree_row_row_cmp_t cmp) {
    int j, Limit;
    off_t sib_addr, tmp_addr;
    BNode* root = (BNode*)get_page(pool, root_addr);

    j = 0;
    while (j < root->n && cmp(rid, root->row_ptr[j]) >= 0) {
        if (cmp(rid, root->row_ptr[j]) == 0)//same rid
            return root_addr;
        j++;
    }
    if (j != 0 && root->child[0] != -1) j--;

    if (root->child[0] == -1) {// leaf node
        release(pool, root_addr);
        root_addr = InsertElement(pool, 1, par_addr, root_addr, rid, i, j);
    }
    else {
        release(pool, root_addr);
        tmp_addr = insert(pool, root->child[j], rid, j, root_addr, cmp);
        root = (BNode*)get_page(pool, root_addr);
        root->child[j] = tmp_addr;
        release(pool, root_addr);
    }

    //modify node
    Limit = 2 * DEGREE;
    root = (BNode*)get_page(pool, root_addr);
    release(pool, root_addr);

    if (root->n > Limit) {
        if (par_addr == -1) {//root, split
            root_addr = SplitNode(pool, par_addr, root_addr, i);
        }
        else {
            sib_addr = FindSibling(pool, par_addr, i);
            if (sib_addr != -1) {//move an element from root to simbling
                MoveElement(pool, root_addr, sib_addr, par_addr, i, 1, cmp);
            }
            else {//split
                root_addr = SplitNode(pool, par_addr, root_addr, i);
            }
        }
    }

    if (par_addr != -1) {
        BNode* parent = (BNode*)get_page(pool, par_addr);
        root = (BNode*)get_page(pool, root_addr);
        release(pool, root_addr);
        parent->row_ptr[i] = root->row_ptr[0];
        release(pool, par_addr);
    }

    return root_addr;
}

void b_tree_close(BufferPool* pool) {
    close_buffer_pool(pool);
}

RID search(BufferPool* pool, void* key, size_t size, b_tree_ptr_row_cmp_t cmp, off_t root_addr) {
    RID null_rid, rid;
    get_rid_block_addr(null_rid) = -1;
    get_rid_idx(null_rid) = 0;
    int j;
    rid = null_rid;

    if (root_addr != -1) {
        BNode* root = (BNode*)get_page(pool, root_addr);
        release(pool, root_addr);
        if (cmp(key, size, root->row_ptr[0]) < 0)
            return null_rid;
        //find branch
        j = 0;
        while (j < root->n && cmp(key, size, root->row_ptr[j]) >= 0) {
            if (cmp(key, size, root->row_ptr[j]) == 0)
                return root->row_ptr[j];
            j++;
        }

        if (root->child[0] == -1) {
            if (cmp(key, size, root->row_ptr[j]) != 0 || j == root->n)
                return null_rid;
        }
        else
            if (j == root->n || cmp(key, size, root->row_ptr[j]) < 0) j--;

        if (root->child[0] == -1) {
            return root->row_ptr[j];
        }
        else {
            rid = search(pool, key, size, cmp, root->child[j]);
        }
    }
    return rid;
}

RID b_tree_search(BufferPool* pool, void* key, size_t size, b_tree_ptr_row_cmp_t cmp) {
    BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
    release(pool, 0);
    off_t root_addr = ctrl->root_node;
    return search(pool, key, size, cmp, root_addr);
}

RID b_tree_insert(BufferPool* pool, RID rid, b_tree_row_row_cmp_t cmp, b_tree_insert_nonleaf_handler_t insert_handler) {
    BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
    release(pool, 0);
    insert(pool, ctrl->root_node, rid, 0, -1, cmp);
    return rid;
}

static off_t delete(BufferPool* pool, off_t root_addr, RID rid, int i, off_t par_addr, b_tree_row_row_cmp_t cmp) {
    int j, NeedAdjust;
    off_t sib_addr, tmp_addr, child_addr;
    BNode* Tmp, * Parent, * Child;
    BCtrlBlock* ctrl;

    sib_addr = -1;
    if (root_addr != -1) {
        BNode* root = (BNode*)get_page(pool, root_addr);
        release(pool, root_addr);
        root = (BNode*)get_page(pool, root_addr);
        //find branch
        j = 0;
        off_t test_rid_addr = get_rid_block_addr(rid);
        off_t test_row_addr = get_rid_block_addr(root->row_ptr[j]);
        while (j < root->n && cmp(rid, root->row_ptr[j]) >= 0) {
            test_rid_addr = get_rid_block_addr(rid);
            test_row_addr = get_rid_block_addr(root->row_ptr[j]);
            if (cmp(rid, root->row_ptr[j]) == 0)
                break;
            j++;
        }
        if (root->child[0] == -1) {
            if (cmp(rid, root->row_ptr[j]) != 0 || j == root->n)
                return root_addr;
        }
        else
            if (j == root->n || cmp(rid, root->row_ptr[j]) < 0) j--;

        if (root->child[0] == -1) {
            release(pool, root_addr);
            root_addr = RemoveElement(pool, 1, par_addr, root_addr, i, j);
        }
        else {
            release(pool, root_addr);
            tmp_addr = delete(pool, root->child[j], rid, j, root_addr, cmp);
            root = (BNode*)get_page(pool, root_addr);
            root->child[j] = tmp_addr;
            release(pool, root_addr);
        }

        NeedAdjust = 0;
        if (par_addr == -1 && root->child[0] != -1 && root->n < 2)
            NeedAdjust = 1;
        else if (par_addr != -1 && root->child[0] != -1 && root->n < LIMIT_DEGREE_2)
            NeedAdjust = 1;
        else if (par_addr != -1 && root->child[0] == -1 && root->n < LIMIT_DEGREE_2)
            NeedAdjust = 1;

        //modify nodes
        if (NeedAdjust) {
            if (par_addr == -1) {//root
                if (root->child[0] != -1 && root->n < 2) {
                    tmp_addr = root_addr;
                    root_addr = root->child[0];

                    //free(Tmp);
                    ctrl = (BCtrlBlock*)get_page(pool, 0);
                    Tmp = (BNode*)get_page(pool, tmp_addr);
                    Tmp->next = ctrl->free_node_head;
                    ctrl->free_node_head = tmp_addr;
                    //clear data
                    Tmp->n = 0;
                    for (int i = 0; i < 2 * DEGREE + 1; i++) {
                        Tmp->child[i] = -1;
                    }
                    for (int i = 0; i < 2 * DEGREE + 1; i++) {
                        get_rid_block_addr(Tmp->row_ptr[i]) = -1;
                        get_rid_idx(Tmp->row_ptr[i]) = 0;
                    }
                    ctrl->root_node = root_addr;
                    release(pool, tmp_addr);
                    release(pool, 0);
                    return root_addr;
                }
            }
            else {
                sib_addr = FindSiblingKeyNum_M_2(pool, par_addr, i, &j);
                if (sib_addr != -1) {
                    MoveElement(pool, sib_addr, root_addr, par_addr, j, 1, cmp);
                }
                else {
                    Parent = (BNode*)get_page(pool, par_addr);
                    release(pool, par_addr);
                    if (i == 0)
                        sib_addr = Parent->child[1];
                    else
                        sib_addr = Parent->child[i - 1];
                    par_addr = MergeNode(pool, par_addr, root_addr, sib_addr, i, cmp);
                    Parent = (BNode*)get_page(pool, par_addr);
                    release(pool, par_addr);
                    root_addr = Parent->child[i];
                }
            }
        }
    }
    if (par_addr != -1) {
        Parent = (BNode*)get_page(pool, par_addr);
        child_addr = Parent->child[i];
        Child = (BNode*)get_page(pool, child_addr);
        Parent->row_ptr[i] = Child->row_ptr[0];
        off_t addr = get_rid_block_addr(Child->row_ptr[0]);
        release(pool, child_addr);
        release(pool, par_addr);
    }
    return root_addr;
}

void b_tree_delete(BufferPool* pool, RID rid, b_tree_row_row_cmp_t cmp, b_tree_insert_nonleaf_handler_t insert_handler, b_tree_delete_nonleaf_handler_t delete_handler) {
    BCtrlBlock* ctrl = (BCtrlBlock*)get_page(pool, 0);
    release(pool, 0);
    delete(pool, ctrl->root_node, rid, 0, -1, cmp);
}