import unittest
from lab1.test.AddTest import AddTest
from lab1.test.DeleteTest import DeleteTest
from lab1.test.SaveOpenTest import SaveOpenTest
from lab1.test.UndoRedoTest import UndoRedoTest
from lab1.test.ShowTest import ShowTest
from lab1.test.LsTest import LsTest
from lab1.test.ReadTest import ReadTest

if __name__ == '__main__':
    loader = unittest.TestLoader()

    add_test = loader.loadTestsFromTestCase(AddTest)
    delete_test = loader.loadTestsFromTestCase(DeleteTest)
    save_open_test = loader.loadTestsFromTestCase(SaveOpenTest)
    undo_redo_test = loader.loadTestsFromTestCase(UndoRedoTest)
    show_test = loader.loadTestsFromTestCase(ShowTest)
    ls_test = loader.loadTestsFromTestCase(LsTest)
    read_test = loader.loadTestsFromTestCase(ReadTest)

    suite = unittest.TestSuite([add_test, delete_test, save_open_test, undo_redo_test, show_test, ls_test, read_test])
    unittest.TextTestRunner().run(suite)
