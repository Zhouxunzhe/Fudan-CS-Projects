#lang scheme

(define (f x)
  (- (+ (* 2 (expt x 2)) x) 4))

; Tests

(define (test x expected)
    (let ((result (f x)))
        (if (= result expected)
            (display (format "Ok, expect ~a got ~a.\n" expected result))
            (error (format "Expect ~a got ~a.\n" expected result)))))

(test -10 186)
(test -9 149)
(test -8 116)
(test -7 87)
(test -6 62)
(test -5 41)
(test -4 24)
(test -3 11)
(test -2 2)
(test -1 -3)
(test 0 -4)
(test 1 -1)
(test 2 6)
(test 3 17)
(test 4 32)
(test 5 51)
(test 6 74)
(test 7 101)
(test 8 132)
(test 9 167)
(test 10 206)