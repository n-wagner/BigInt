/*************************************************************************
 *                                                                       *
 *  This Source Code Form is subject to the terms of the Mozilla Public  *
 *  License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 *                                                                       *
 *************************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;


long long int factorial (const long long int num) {
  if (num > 1) {
    return num * factorial(num - 1);
  } else {
    return 1;
  }
}

long long int reduced_factorial (const long long int num, const long long int stop) {
  if (num == stop || num <= 1) {
    return 1;
  } else {
    return num * reduced_factorial(num - 1, stop);
  }
}

long long int n_choose_k (const long long int n, const long long int k) {
  const int difference = n - k;
  const int stop = k > difference ? k : difference;

  return reduced_factorial(n, stop) / factorial(n - stop);
}

int main () {
  const int max = 20;

  for (int n = 0; n <= max; n++) {
    cout << "N = " << setw(2) << n << "; ";
    for (int k = 0; k <= n; k++) {
      cout << n_choose_k(n, k) << ' ';
    }
    cout << endl;
  }

  long long int previous;
  for (int n = 0; n <= max; n++) {
    long long int fac = factorial(n), n_k = n_choose_k(n, n / 2), fac_n_k = fac / n_k;
    cout << "N = " << setw(2) << n << "; "; 
//    cout << fac << ' ' << n_k << ' ' << fac_n_k << ' ';
    if (n > 0) {
      int m = n - 1;
      cout << fac_n_k << ' ' << fac_n_k / previous << ' ';
      cout << (factorial(n / 2) * factorial(n - (n / 2))) / (factorial(m / 2) * factorial(m - (m / 2)));
    }
    cout << endl;
    previous = fac_n_k;
  }

  return 0;
}
