#lang scheme

(define (is-prime? n)
  (if (or (< n 2) (= n 2)) 
      (if (= n 2) #t #f)
      (let loop ((i 2))
        (cond ((> (* i i) n) #t)
              ((= (remainder n i) 0) #f)
              (else (loop (+ i 1)))))))

(define (count-primes-in n)
  (define (count-primes-iter i count)
    (if (> i n)
        count
        (count-primes-iter (+ i 1) (if (is-prime? i) (+ count 1) count))))
  (count-primes-iter 2 0))

; Tests

(define (test n expected)
    (let ((result (count-primes-in n)))
        (if (= result expected)
            (display (format "Ok, expect ~a got ~a.\n" expected result))
            (error (format "Expect ~a got ~a.\n" expected result)))))

(test 2 1)
(test 3 2)
(test 4 2)
(test 5 3)
(test 6 3)
(test 7 4)
(test 8 4)
(test 9 4)
(test 10 4)
(test 6011 785)
(test 114514 10830)
(test 321299 27709)
(test 377386 32097)
(test 675606 54733)
(test 765249 61378)