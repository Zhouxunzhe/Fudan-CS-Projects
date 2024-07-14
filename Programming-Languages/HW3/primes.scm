#lang scheme

(define (prime?-helper x i)
  (if (> (* i i) x)
      1
      (if (= (remainder x i) 0)
          0
          (prime?-helper x (+ i 1)))))

; assume x>=2
(define (prime? x) (prime?-helper x 2))

(define (count-primes-in n)
  (if (= n 1)
      0
      (+ (count-primes-in (- n 1))
         (prime? n))))

; Tests

(define (test result expected)
  (if (equal? result expected)
      (display (format "Ok, expect ~a got ~a.\n" expected result))
      (error (format "Expect ~a got ~a.\n" expected result))))

(test (count-primes-in 2) 1)
(test (count-primes-in 3) 2)
(test (count-primes-in 4) 2)
(test (count-primes-in 5) 3)
(test (count-primes-in 6) 3)
(test (count-primes-in 7) 4)
(test (count-primes-in 8) 4)
(test (count-primes-in 9) 4)
(test (count-primes-in 10) 4)
(test (count-primes-in 6011) 785)
(test (count-primes-in 114514) 10830)