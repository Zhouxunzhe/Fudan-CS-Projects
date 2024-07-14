#lang scheme

(define (even? x)
  (= (remainder x 2) 0))

(define (fast-exp-mod x y p)
  (define (helper xn yn acc)
    (cond ((= yn 0) (remainder acc p))
          ((even? yn) (helper (remainder (* xn xn) p)
                              (/ yn 2)
                              acc))
          (else (helper (remainder (* xn xn) p)
                        (/ (- yn 1) 2)
                        (remainder (* acc xn) p)))))
  (helper x y 1))

(define (cond-actions-test x)
  (cond ((= x 0) (define y 5) y)
        ((= x 1) 'b)
        ((= x 2) 'c)
        (else (define y (+ x 2))
              (define z (* x 3))
              (+ y z))))

; Tests

(define (test result expected)
  (if (equal? result expected)
      (display (format "Ok, expect ~a got ~a.\n" expected result))
      (error (format "Expect ~a got ~a.\n" expected result))))

(test (fast-exp-mod 2 10 17) 4)
(test (fast-exp-mod 3 10 17) 8)
(test (fast-exp-mod 4 10 17) 16)
(test (fast-exp-mod 5 10 17) 9)
(test (fast-exp-mod 6 100 998244353) 943484039)
(test (fast-exp-mod 7 100 998244353) 680223855)
(test (fast-exp-mod 8 100 998244353) 465147789)
(test (fast-exp-mod 9 100 998244353) 501681095)
(test (fast-exp-mod 10 100 998244353) 876867878)
(test (fast-exp-mod 4179847 0 10009) 1)
(test (fast-exp-mod 1 0 1) 0)
(test (fast-exp-mod 1919810 114514 998244353) 396771331)
(test (fast-exp-mod 19990210 1000000000 19260817) 15311107)

(test (cond-actions-test 0) 5)
(test (cond-actions-test 1) 'b)
(test (cond-actions-test 2) 'c)
(test (cond-actions-test 5) 22)