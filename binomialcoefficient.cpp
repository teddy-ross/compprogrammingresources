const int MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = res * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return res;
}

long long C(int n, int k) {
    if (k > n) return 0;
    long long num = 1, den = 1;
    for (int i = 0; i < k; ++i) {
        num = num * ((n - i) % MOD) % MOD;
        den = den * ((i + 1) % MOD) % MOD;
    }
    return num % MOD * power(den, MOD - 2, MOD) % MOD;
}
