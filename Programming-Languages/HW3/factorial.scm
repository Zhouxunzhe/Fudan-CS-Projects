#lang scheme

(define (factorial n)
  (if (= n 0)
      1
      (* (factorial (- n 1)) n)))

; Tests

(define (test result expected)
  (if (equal? result expected)
      (display (format "Ok, expect ~a got ~a.\n" expected result))
      (error (format "Expect ~a got ~a.\n" expected result))))

(test (factorial 1) 1)
(test (factorial 2) 2)
(test (factorial 3) 6)
(test (factorial 5) 120)
(test (factorial 10) 3628800)