#lang scheme

(define (test result expected)
    (if (equal? result expected)
        (display (format "Ok, expect ~a got ~a.\n" expected result))
        (error (format "Expect ~a got ~a.\n" expected result))))

(provide test)