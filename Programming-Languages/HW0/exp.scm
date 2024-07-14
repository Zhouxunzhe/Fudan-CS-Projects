#lang scheme

(define (fast-exp-mod x y p)
  (if (= p 1) 0
      (letrec ((expmod-iter (lambda (x y z p)
                              (cond ((= y 0) z)
                                    ((even? y)
                                     (expmod-iter (remainder (* x x) p) (/ y 2) z p))
                                    (else
                                     (expmod-iter x (- y 1) (remainder (* z x) p) p))))))
        (expmod-iter x y 1 p))))

(define (even? n) (= (remainder n 2) 0))

; Tests

(define (test x y mod expected)
    (let ((result (fast-exp-mod x y mod)))
        (if (= result expected)
            (display (format "Ok, expect ~a got ~a.\n" expected result))
            (error (format "Expect ~a got ~a.\n" expected result)))))

(test 2 10 17 4)
(test 3 10 17 8)
(test 4 10 17 16)
(test 5 10 17 9)
(test 6 100 998244353 943484039)
(test 7 100 998244353 680223855)
(test 8 100 998244353 465147789)
(test 9 100 998244353 501681095)
(test 10 100 998244353 876867878)
(test 4179847 0 10009 1)
(test 1 0 1 0)
(test 1919810 114514 998244353 396771331)
(test 19990210 1000000000 19260817 15311107)