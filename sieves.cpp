pair<vector<int>, vector<int>> linear_sieve(const int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
        for (int p : primes) {
            if (p > spf[i] || (long long)i * p > n) break;
            spf[i * p] = p;
        }
    }
    return {spf, primes};

vector<bool> sieve(const int n){
    vector<bool> primes(n + 1, true);
    primes[0] = false;
    if(n > 1) primes[1] = false;

    for(int i = 2; i < n; ++i){
        if(!isPrime) continue;
        // inner loop goes up in multiples of chosen number
        for(int j = i * i; j <= n; j += i){
            isPrime[j] = false;
        }
    }

    return primes;
}
