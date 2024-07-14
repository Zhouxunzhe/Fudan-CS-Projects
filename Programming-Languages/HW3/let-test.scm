#lang scheme

(define (silly-let-1)
  (let ((x 1) (y (+ 1 1)) (z (* 2 3)))
    (+ (+ (+ x y) z)
       (let ((x 2) (z 4))
         (+ (+ x z) y)))))

(define (silly-let-2)
  (let () 114514))

(define (silly-let-3)
  (let ((f (lambda (x) (+ x 2))))
    (f 3)))

; Tests

(define (test result expected)
  (if (equal? result expected)
      (display (format "Ok, expect ~a got ~a.\n" expected result))
      (error (format "Expect ~a got ~a.\n" expected result))))

(test (silly-let-1) 17)
(test (silly-let-2) 114514)
(test (silly-let-3) 5)
