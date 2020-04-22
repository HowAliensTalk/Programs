
import java.util.ArrayList;
import java.util.Arrays;

/**
 * File : Primes.java
 *
 * This is a research project in which I am attempting to determine some kind of
 * pattern embedded in a sequence of prime numbers
 *
 * Results of testing are made in entry in the analysis.txt file in this folder
 *
 * @author dtgmu
 */
public class Primes {

    public static ArrayList<Integer> knownPrimes = new ArrayList<>();

    public static void main(String[] args) {
        int n = 12;

        /**
         * If we take largest prime less than n modulus the position at which
         * the prime was found will the resulting number also be prime?
         */
        // Primes less than array
        ArrayList<Integer> p = new ArrayList<>();

        // index of max prime of p
        int r;

        // largest prime of p
        int l;

        // 
        for (int i = 3; i < n; i++) {
            if (prime(i)) {
                p = primesLessThan(i);
                r = p.size() - 1;
                l = p.indexOf(p.size() - 1);
                System.out.println(l);
                System.out.println(i + " % " + r + " = " + i % r);
            }

        }
        System.out.println(p);

    }

    /**
     * Checks if given number is prime
     *
     * @param x
     * @return true if prime
     */
    static boolean prime(int x) {

        // our stopping point is half of 'x'
        int stop = x / 2;

        // begin at 2 since 1 is factor of all numbers
        for (int i = 2; i <= stop; i++) {
            if (x % i == 0) {
                return false;
            }
        }

        return true;
    }

    static ArrayList<Integer> primesLessThan(int x) {

        // CASE 1 : x < last known prime in knownPrimes
        // No need to check if any numbers are prime if x is less than already known primes
        if (knownPrimes.isEmpty()){
            for(int i = 1; i < x; i++){
                if(prime(i)){
                    knownPrimes.add(i);
                }
            }
          return knownPrimes;
        }
        else if (knownPrimes.get(size() - 1) > x) {

            int p = knownPrimes.get(size() - 1);
            int j = 1;
            while (x > p) {
                p = knownPrimes.get(size() - ++j);
            }

            return (ArrayList<Integer>) knownPrimes.subList(0, p);
        } // CASE 2 : x > last known prime in knownPrimes
        else {
            int lastPrime = knownPrimes.get(size() - 1);

            // Start at last prime since x > last known prime
            for (int i = lastPrime + 1; i < x; i++) {
                if (prime(i)) {
                    knownPrimes.add(i);
                }
            }

            return knownPrimes;
        }
    }

    static int size() {
        return knownPrimes.size();
    }
}
