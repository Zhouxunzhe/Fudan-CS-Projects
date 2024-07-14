#lang scheme

(require r5rs)
(print-mpair-curly-braces #f)

; tagged list
(define (tagged-list? exp tag)
  (and (pair? exp) (eq? (car exp) tag)))

; ==================================================
; evaluator structers

(define variable? symbol?)

; frame & environment
(define enclosing-environment cdr)
(define first-frame car)
(define the-empty-environment '())
(define (make-frame variables values)
  (cons variables values))
(define frame-variables car)
(define frame-values cdr)
(define (add-binding-to-frame! var val frame)
  (set-car! frame (cons var (car frame)))
  (set-cdr! frame (cons val (cdr frame))))
(define (extend-environment vars vals base-env)
  (if (= (length vars) (length vals))
      (cons (make-frame vars vals) base-env)
      (if (< (length vars) (length vals))
          (error "Too many arguments supplied" vars vals)
          (error "Too few arguments supplied" vars vals))))
(define (lookup-variable-value var env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars) (env-loop (enclosing-environment env)))
            ((eq? var (car vars)) (car vals))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable" var)
        (let ((frame (first-frame env)))
          (scan (frame-variables frame)
                (frame-values frame)))))
  (env-loop env))
(define (set-variable-value! var val env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars) (env-loop (enclosing-environment env)))
            ((eq? var (car vars)) (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable -- SET!" var)
        (let ((frame (first-frame env)))
          (scan (frame-variables frame)
                (frame-values frame)))))
  (env-loop env))
(define (define-variable! var val env)
  (let ((frame (first-frame env)))
    (define (scan vars vals)
      (cond ((null? vars) (add-binding-to-frame! var val frame))
            ((eq? var (car vars)) (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (scan (frame-variables frame)
          (frame-values frame))))

; self-evaluated
(define (self-evaluating? exp)
  (cond ((number? exp) #t)
        ((string? exp) #t)
        (else #f)))

; primitive procedures & primitive values & global environment
(define (true? x) (not (false? x)))
(define (false? x) (equal? x false))
(define (primitive-procedure? proc)
  (tagged-list? proc 'primitive))
(define primitive-procedure-implementation cadr)
(define primitive-procedures
  (list (list 'car car)
        (list 'cdr cdr)
        (list 'cons cons)
        (list 'null? null?)
        (list 'list list)
        (list '+ +)
        (list '- -)
        (list '* *)
        (list '/ /)
        (list '= =)
        (list '> >)
        (list '>= >=)
        (list '< <)
        (list '<= <=)
        (list 'not not)
        (list 'eq? eq?)
        (list 'equal? equal?)
        (list 'eqv eqv?)
        (list 'remainder remainder)
        (list 'display display)
        (list 'error error)
        (list 'format format)
        (list 'exit exit)
        ;; you can add more primitive procedures here
        ))
(define (primitive-procedure-names)
  (map car primitive-procedures))
(define (primitive-procedure-objects)
  (map (lambda (proc) (list 'primitive (cadr proc)))
       primitive-procedures))
(define (apply-primitive-procedure proc args)
  (apply (primitive-procedure-implementation proc) args))
(define (setup-environment)
  (let ((initial-env (extend-environment (primitive-procedure-names)
                                         (primitive-procedure-objects)
                                         the-empty-environment)))
    (define-variable! 'true true initial-env)
    (define-variable! 'false false initial-env)
    initial-env))
(define the-global-environment (setup-environment))

; procedure
(define (make-procedure parameters body env)
  (list 'procedure parameters body env))
(define (compound-procedure? exp)
  (tagged-list? exp 'procedure))
(define procedure-parameters cadr)
(define procedure-body caddr)
(define procedure-environment cadddr)

; ==================================================
; expressions

; assignment
(define (assignment? exp)
  (tagged-list? exp 'set!))
(define (assignment-variable exp) (cadr exp))
(define (assignment-value exp) (caddr exp))
(define (eval-assignment exp env)
  (set-variable-value! (assignment-variable exp)
                       (m-eval (assignment-value exp) env)
                       env)
  'ok)

; defination
(define (definition? exp)
  (tagged-list? exp 'define))
(define (definition-variable exp)
  (if (symbol? (cadr exp))
      (cadr exp)
      (caadr exp)))
(define (definition-value exp)
  (if (symbol? (cadr exp))
      (caddr exp)
      (make-lambda (cdadr exp) (cddr exp))))
(define (eval-definition exp env)
  (define-variable! (definition-variable exp)
    (m-eval (definition-value exp) env)
    env)
  'ok)

; lambda
(define (lambda? exp)
  (tagged-list? exp 'lambda))
(define (make-lambda parameters body)
  (cons 'lambda (cons parameters body)))
(define (lambda-parameters exp) (cadr exp))
(define (lambda-body exp) (cddr exp))

; quote
(define (quoted? exp)
  (tagged-list? exp 'quote))
(define (text-of-quatation exp) (cadr exp))

; if
(define (if? exp)
  (tagged-list? exp 'if))
(define if-predicate cadr)
(define if-consequent caddr)
(define (if-alternative exp)
  (if (not (null? (cdddr exp)))
      (cadddr exp)
      'false))
(define (eval-if exp env)
  (if (true? (m-eval (if-predicate exp) env))
      (m-eval (if-consequent exp) env)
      (m-eval (if-alternative exp) env)))
(define (make-if predicate consequent alternative) ;; for cond
  (list 'if predicate consequent alternative))

; begin
(define (begin? exp)
  (tagged-list? exp 'begin))
(define (make-begin exps)
  (cons 'begin exps))
(define (last-exp? seq) (null? (cdr seq)))
(define first-exp car)
(define rest-exps cdr)
(define begin-actions cdr)
(define (sequence->exp seq)  ;; for cond
  (cond ((null? seq) seq)
        ((last-exp? seq) (first-exp seq))
        (else (make-begin seq))))

; sequence
(define (eval-sequence exps env)
  (cond ((last-exp? exps) (m-eval (first-exp exps) env))
        (else (m-eval (first-exp exps) env)
              (eval-sequence (rest-exps exps) env))))

; application
(define application? pair?)
(define operator car)
(define operands cdr)
(define (list-of-values exps env)
  (map (lambda (exp) (m-eval exp env)) exps))
(define (m-apply procedure arguments)
  (cond ((primitive-procedure? procedure)
         (apply-primitive-procedure procedure arguments))
        ((compound-procedure? procedure)
         (eval-sequence (procedure-body procedure)
                        (extend-environment (procedure-parameters procedure)
                                            arguments
                                            (procedure-environment procedure))))
        (else (error "Unkown procedure type -- APPLY" procedure))))

; cond
;; your code here
(define (cond-clauses exp) (cdr exp))
(define (cond-else-clause? clause)
  (eq? (cond-predicate clause) 'else))
(define (cond-predicate clause) (car clause))
(define (cond-actions clause) (cdr clause))
(define (cond->if exp)
  (expand-clauses (cond-clauses exp)))

(define (expand-clauses clauses)
  (if (null? clauses)
      'false  ; no else clause
      (let ((first (car clauses))
            (rest (cdr clauses)))
        (if (cond-else-clause? first)
            (if (null? rest)
                (sequence->exp (cond-actions first))
                (error "ELSE clause isn't last -- COND->IF" clauses))
            (make-if (cond-predicate first)
                     (sequence->exp (cond-actions first))
                     (expand-clauses rest))))))

(define (eval-cond exp env)
  (m-eval (cond->if exp) env))

(define (cond? exp)
  (tagged-list? exp 'cond))

; let
;; your code here
(define (let? exp)
  (tagged-list? exp 'let))
(define (let-bindings exp) (cadr exp))
(define (let-body exp) (cddr exp))

(define (let->combination exp)
  (cons (make-lambda (map car (let-bindings exp)) (let-body exp))
        (map cadr (let-bindings exp))))

(define (eval-let exp env)
  (m-eval (let->combination exp) env))

; let*
;; your code here
(define (let*? exp)
  (tagged-list? exp 'let*))

(define (let*-bindings exp)
  (cadr exp))

(define (let*-body exp)
  (cddr exp))

(define (eval-let* exp env)
  (define (eval-let*-bindings bindings env)
    (if (null? bindings)
        env
        (let ((first-binding (car bindings)))
          (let ((var (car first-binding))
                (val (m-eval (cadr first-binding) env)))
            (eval-let*-bindings (cdr bindings)
                                 (extend-environment (list var) (list val) env))))))
  (let ((new-env (eval-let*-bindings (let*-bindings exp) env)))
    (eval-sequence (let*-body exp) new-env)))

; ==================================================
; metacircular evaluator

(define (m-eval exp env)
  (cond
    ; primitives
    ((self-evaluating? exp) exp)
    ((variable? exp) (lookup-variable-value exp env))
    ((quoted? exp) (text-of-quatation exp))
    ; special forms
    ((assignment? exp) (eval-assignment exp env))
    ((definition? exp) (eval-definition exp env))
    ((if? exp) (eval-if exp env))
    ((lambda? exp) (make-procedure (lambda-parameters exp)
                                   (lambda-body exp)
                                   env))
    ((begin? exp) (eval-sequence (begin-actions exp) env))
    ;; eval cond here
    ((cond? exp) (eval-cond exp env))
    ;; eval let here
    ((let? exp) (eval-let exp env))
    ;; eval let* here
    ((let*? exp) (eval-let* exp env))
    ; application
    ((application? exp) (m-apply (m-eval (operator exp) env)
                                 (list-of-values (operands exp) env)))
    (else (error "Unkown expression type -- EVAL" exp))))

; ==================================================
; dirvers

(define input-prompt ";;; M-Eval input:")
(define output-prompt ";;; M-Eval value:")
(define (prompt-for-input string)
  (newline) (newline) (display string) (newline))
(define (announce-output string)
  (newline) (display string) (newline))

(define (driver-loop) (repl #f))

(define (repl port)
  (if port #f (prompt-for-input input-prompt))
  (let ((input (if port (read port) (read))))
    (cond ((eof-object? input)   'meval-done)
          ((eq? input '**quit**) 'meval-done)
          (else
           (let ((output (m-eval input the-global-environment)))
             (if port #f (begin
                           (announce-output output-prompt)
                           (pretty-display output)))
             (repl port))))))

(define (m-eval-file file-name)
  (let ((stream (open-input-file file-name)))
    (read-line stream) ;; strip off "#lang scheme" line
    (repl stream)))    ;; feed the rest of the definitions into m-eval

(define (main)
  (let ((args (vector->list (current-command-line-arguments))))
    (define (run-files files)
      (cond ((null? files) 'done)
            (else (m-eval-file (car files))
                  (display  (format "~a done." (car files)))
                  (newline)
                  (run-files (cdr files)))))
    (if (null? args)
        (driver-loop)
        (run-files args))
    (newline)))

(main)