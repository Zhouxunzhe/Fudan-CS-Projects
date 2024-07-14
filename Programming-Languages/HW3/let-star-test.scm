#lang scheme

(define (silly-let-star-1)
  (let* ((x 3)
         (y (+ x 2))
         (z (+ x y 5)))
    (* x z)))

(define (silly-let-star-2)
  (let* ((f (lambda (x) (* 2 x)))
         (g (lambda (x y) (+ x y)))
         (z (g (f 5) (f 6))))
    (+ (f 3) z)))

; Tests

(define (test result expected)
  (if (equal? result expected)
      (display (format "Ok, expect ~a got ~a.\n" expected result))
      (error (format "Expect ~a got ~a.\n" expected result))))

(test (silly-let-star-1) 39)
(test (silly-let-star-2) 28)
