bool anyTrue(const double a[], int n) {
    if(n<=0) return false;
    if(somePredicate(*a)) return true;
    return anyTrue(a+1, n-1);
}


int countTrue(const double a[], int n) {
    if(n<=0) return 0;
    if(somePredicate(*a)) return (1 + countTrue(a+1, n-1));
    return countTrue(a+1, n-1);
}

int firstTrue(const double a[], int n) {
    if(n<=0) return -1;
    int p = firstTrue(a+1, n-1);
    if(p == -1) {
        if(somePredicate(*a)) {
            return 0;
        }
        return -1;
    }
    if(somePredicate(*a)) {
        return 0;
    }
    return 1+p;
}

int indexOfMinimum(const double a[], int n) {
    if(n<=0) return -1;
    if(n == 1) return 0;
    int middle = n/2;
    int minFirstHalf = indexOfMinimum(a, middle);
    int minSecondHalf = indexOfMinimum(a+middle,n-middle)+middle;
    if(a[minFirstHalf] < a[minSecondHalf]) return minFirstHalf;
    return minSecondHalf;
}


bool isIn(const double a1[], int n1, const double a2[], int n2) {
    if(n1<=0) return true;
    if(n1>n2) return false;
    if(a1[0] == a2[0]) {
        return isIn(a1+1, n1-1, a2+1, n2-1);
    }
    return isIn(a1, n1, a2+1, n2-1);
}
