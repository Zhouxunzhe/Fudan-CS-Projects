#lang scheme

(require compatibility/mlist)
(require "test.scm")

; Constructor
; make-table :: () -> Table
(define (make-table)
  (mlist 'Table))

; Predicate
; table? :: Table -> Bool
(define (table? table)
  (and (mpair? table)
       (eq? (mcar table) 'Table)))

; Operations
; table-put! :: (Table, a, b) -> Table
(define (table-put! table key value)
  (let ((pairs (mcdr table)))  ; Access the cdr, which should be the association list.
    (let ((existing-pair (massoc key pairs)))
      (if existing-pair
          (set-mcdr! existing-pair value)  ; Update existing value
          (set-mcdr! table (mcons (mcons key value) pairs))))))

; table-has-key? :: (Table, a) -> Bool
(define (table-has-key? table key)
  (let ((existing-pair (massoc key (mcdr table))))  ; Use mcdr to get the association list from table
    (not (equal? existing-pair #f))))

; table-get :: (Table, a) -> b
(define (table-get table key)
  (let ((existing-pair (massoc key (mcdr table))))  ; Use mcdr to get the association list from table
    (if existing-pair
        (mcdr existing-pair)
        #f)))

(define mod 19260817)

; fib :: Int -> Int
; Calculate the n-th term of the Fibonacci sequence (A000045) mod 19260817
(define (fib n)
  (if (< n 2)
      (remainder n mod)
      (remainder (+ (fib (- n 1)) (fib (- n 2))) mod)))

; narayana :: Int -> Int
; Calculate the n-th term of the Narayana's cows sequence (A000930) mod 19260817
(define (narayana n)
  (if (< n 3)
      1
      (remainder (+ (narayana (- n 1)) (narayana (- n 3))) mod)))

; memoize-1 :: (a -> a) -> (a -> a)
(define (memoize-1 f)
  (let ((table (make-table)))  ; Create a table to store the memoized results.
    (lambda (x)
      (let ((result (table-get table x)))
        (if result  ; If result is not #f, we've computed this before.
            result  ; Return the memoized result.
            (let ((new-result (f x)))  ; Otherwise, compute the result.
              (table-put! table x new-result)  ; Store the new result in the table.
              new-result))))))

(set! fib (memoize-1 fib))
(set! narayana (memoize-1 narayana))

; pascal :: (Int, Int) -> Int
; Calculate the element in row n and column n of the Pascal's triangle (A007318) mod 19260817
(define (pascal n m)
  (if (or (= m 0) (= m n))
      1
      (remainder (+ (pascal (- n 1) (- m 1))
                    (pascal (- n 1) m))
                 mod)))

; stirling :: (Int, Int) -> Int
; Calculate the element in row n and column n of the triangle of Stirling numbers of the second kind (A008277) mod 19260817
(define (stirling n m)
  (cond ((and (= n 0) (= m 0)) 1)
        ((or (= n 0) (= m 0)) 0)
        (else (remainder (+ (* m (stirling (- n 1) m))
                            (stirling (- n 1) (- m 1)))
                         mod))))

; memoize-2 :: ((a, a) -> a) -> ((a, a) -> a)
(define (memoize-2 f)
  (let ((table (make-table)))  ; Create a table to store the memoized results.
    (lambda (x y)
      (let ((args (cons x y)))  ; Define args within the lambda function scope.
        (let ((result (table-get table args)))  ; Try to get the result from the table using args.
          (if result
              result  ; If the result is in the table, return it.
              (let ((new-result (f x y)))  ; Compute the result with f.
                (table-put! table args new-result)  ; Store the new result in the table using args.
                new-result)))))))

(set! pascal (memoize-2 pascal))
(set! stirling (memoize-2 stirling))

; Tests
(define (test-table-1)
  (define t (make-table))
  (test (table-has-key? t 'x) false)
  (test (table-has-key? t 'y) false)
  (table-put! t 'x 123)
  (test (table-has-key? t 'x) true)
  (test (table-has-key? t 'y) false)
  (test (table-get t 'x) 123)
  (table-put! t 'x 234)
  (test (table-get t 'x) 234)
  (display "Ok, test-table-1 passed.\n"))

(define (test-table-2)
  (define t (make-table))
  (table-put! t 'x 1)
  (table-put! t 'y 2)
  (table-put! t 'z 3)
  (table-put! t 1 4)
  (table-put! t 2 5)
  (table-put! t 3 6)
  (test (table-has-key? t 'x) true)
  (test (table-has-key? t 'y) true)
  (test (table-has-key? t 'z) true)
  (test (table-has-key? t 1) true)
  (test (table-has-key? t 4) false)
  (test (table-get t 'x) 1)
  (test (table-get t 'y) 2)
  (test (table-get t 'z) 3)
  (test (table-get t 1) 4)
  (test (table-get t 2) 5)
  (test (table-get t 3) 6)
  (table-put! t 1 1)
  (table-put! t 2 2)
  (table-put! t 3 3)
  (test (table-get t 'x) 1)
  (test (table-get t 'y) 2)
  (test (table-get t 'z) 3)
  (test (table-get t 1) 1)
  (test (table-get t 2) 2)
  (test (table-get t 3) 3)
  (display "Ok, test-table-2 passed.\n"))

(define (test-memo-fib)
  (test (fib 10) 55)
  (test (fib 100) 15426709)
  (test (fib 1000) 8161161)
  (test (fib 10000) 2596925)
  (display "Ok, test-memo-fib passed.\n"))

(define (test-memo-narayana)
  (test (narayana 10) 28)
  (test (narayana 100) 15535004)
  (test (narayana 1000) 16964023)
  (test (narayana 10000) 18255303)
  (display "Ok, test-memo-narayana passed.\n"))

(define (test-memo-pascal)
  (test (pascal 0 0) 1)
  (test (pascal 1 0) 1)
  (test (pascal 1 1) 1)
  (test (pascal 2 0) 1)
  (test (pascal 2 1) 2)
  (test (pascal 2 2) 1)
  (test (pascal 3 0) 1)
  (test (pascal 3 1) 3)
  (test (pascal 3 2) 3)
  (test (pascal 3 3) 1)
  (test (pascal 4 0) 1)
  (test (pascal 4 1) 4)
  (test (pascal 4 2) 6)
  (test (pascal 4 3) 4)
  (test (pascal 4 4) 1)
  (test (pascal 5 0) 1)
  (test (pascal 5 1) 5)
  (test (pascal 5 2) 10)
  (test (pascal 5 3) 10)
  (test (pascal 5 4) 5)
  (test (pascal 5 5) 1)
  (test (pascal 6 0) 1)
  (test (pascal 6 1) 6)
  (test (pascal 6 2) 15)
  (test (pascal 6 3) 20)
  (test (pascal 6 4) 15)
  (test (pascal 6 5) 6)
  (test (pascal 6 6) 1)
  (test (pascal 10 6) 210)
  (test (pascal 37 16) 9548914)
  (test (pascal 45 12) 3621964)
  (test (pascal 100 10) 16133213)
  (test (pascal 100 50) 12391455)
  (test (pascal 100 90) 16133213)
  (test (pascal 100 100) 1)
  (test (pascal 100 0) 1)
  (test (pascal 1000 0) 1)
  (test (pascal 1000 3) 12080464)
  (display "Ok, test-memo-pascal passed.\n"))

(define (test-memo-stirling)
  (test (stirling 0 0) 1)
  (test (stirling 1 0) 0)
  (test (stirling 1 1) 1)
  (test (stirling 2 0) 0)
  (test (stirling 2 1) 1)
  (test (stirling 2 2) 1)
  (test (stirling 3 0) 0)
  (test (stirling 3 1) 1)
  (test (stirling 3 2) 3)
  (test (stirling 3 3) 1)
  (test (stirling 4 0) 0)
  (test (stirling 4 1) 1)
  (test (stirling 4 2) 7)
  (test (stirling 4 3) 6)
  (test (stirling 4 4) 1)
  (test (stirling 5 0) 0)
  (test (stirling 5 1) 1)
  (test (stirling 5 2) 15)
  (test (stirling 5 3) 25)
  (test (stirling 5 4) 10)
  (test (stirling 5 5) 1)
  (test (stirling 6 0) 0)
  (test (stirling 6 1) 1)
  (test (stirling 6 2) 31)
  (test (stirling 6 3) 90)
  (test (stirling 6 4) 65)
  (test (stirling 6 5) 15)
  (test (stirling 6 6) 1)
  (test (stirling 10 6) 22827)
  (test (stirling 37 16) 15572435)
  (test (stirling 45 12) 17619323)
  (test (stirling 100 10) 173828)
  (test (stirling 100 50) 6745772)
  (test (stirling 100 90) 7188850)
  (test (stirling 100 100) 1)
  (test (stirling 100 0) 0)
  (test (stirling 1000 0) 0)
  (test (stirling 1000 3) 12271060)
  (display "Ok, test-memo-stirling passed.\n"))

(test-table-1)
(test-table-2)
(test-memo-fib)
(test-memo-narayana)
(test-memo-pascal)
(test-memo-stirling)
(display "Ok, all tests passed.\n")