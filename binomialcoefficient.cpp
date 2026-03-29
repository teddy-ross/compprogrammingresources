unsigned long long binomialCoeff(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    // Use the property C(n, k) = C(n, n-k) to reduce iterations
    if (k > n / 2) k = n - k;

    // Use double for intermediate calculations to prevent overflow
    double res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }

    // Cast the result to unsigned long long (or appropriate integer type)
    return (unsigned long long)(res + 0.01); // Add a small epsilon for rounding
}
