/*************************************************************************
 *                                                                       *
 *  This Source Code Form is subject to the terms of the Mozilla Public  *
 *  License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 *                                                                       *
 *************************************************************************/

#include <vector>
#include <iostream>

using namespace std;


class bInt: public vector<int> {
  public:
    bool negative;

    bInt () {			//Constructor initializes number to positive
      negative = false;
    }

    void newNum (string s) {	//Converts number string to vector of integer digits
      clear();
      negative = false;
      int end = 0;
      if (s[0] == '-'){
        end = 1;
        negative = true;
      } else if (s[0] == '+') {
        end = 1;
      }
      for (int i = s.length() - 1; i >= end; i--) {
        int digval = s[i] - '0';
        if (digval < 0 || digval > 9) {
          cerr << "Invalid digit entry.\n";
          abort();
        }
        push_back(digval);
      }
    }
    
    void print () {		//Prints out a big integer
      if (negative) {
        cout << "-";
      }
      for (int i = size() - 1; i >= 0; i--) {
        cout << at(i);
      }
      cout << endl;
    }

    int digit (int pos) {	//Gives the particular digit at a given position (pos)
      if (pos >= size()) {
        return 0;
      }
      return at(pos);
    }
    
    void set (int pos, int newVal) {	//Sets a particular digit at a given position to a new value
      if (pos == size()) {
        push_back(newVal);
      } else {
        at(pos) = newVal;
      }
    }

    void increment () {			//Increments a big integer by 1
      int pos = 0;
      while (digit(pos) == 9) {
        set (pos, 0);
        pos++;
      }
      set(pos, digit(pos) + 1);
    }

    void trimLeadZero () {		//Removes leading zeros
      int i = size() - 1;
      while (digit(i) == 0 && i != 0) {
        pop_back();
        i--;
      }
    }

    void decrement () {			//Decrements a big integer by 1
      int pos = 0;
      while (digit(pos) == 0) {
        set (pos, 9);
        pos++;
      }
      set(pos, digit(pos) - 1);
      trimLeadZero();
    }

    void nineComp () {			//Preforms the 9's compliment on a big integer
      for (int i = 0; i < size(); i++) {
        set(i, 9 - digit(i));
      }
    }

    bool isOne () {			//Returns true if the big integer == 1
      if (size() == 1 && digit(0) == 1) {
        return true;
      } else {
        return false;
      }
    }

    void add_pos (bInt & A, bInt & B) {		//Adds 2 positive big integers
      clear();
      negative = false;
      int pos = 0, carry = 0;
      while (pos < A.size() || pos < B.size() || carry > 0) {
        int sum = A.digit(pos) + B.digit(pos) + carry;
        carry = sum / 10;
        push_back(sum % 10);
        pos++;
      }
    }

    void subtract_pos (bInt & A, bInt & B) {	//Subtracts 2 positive big integers
      clear();
      negative = false;
      int pos = 0, borrow = 0;
      while (pos < A.size() || pos < B.size()) {
        int diff = A.digit(pos) - B.digit(pos) - borrow;
        if (diff < 0) {
          diff += 10;
          borrow = 1;
        } else {
          borrow = 0;
        }
        push_back(diff);
        pos++;
      }
      if (borrow != 0) {
        //cout << "Borrow still set\n";
        nineComp();
        increment();
        negative = true;
      }
      trimLeadZero();
    }

    void add (bInt & A, bInt & B) {	//Adds 2 big integers
      if (A.negative && B.negative) {
        add_pos(A, B);
        negative = true;
      } else if (A.negative) {
        subtract_pos(B, A);
      } else if (B.negative) {
        subtract_pos(A, B);
      } else {
        add_pos(A, B);
      }
    }

    void subtract (bInt & A, bInt & B) {	//Subtracts 2 big integers
      if (A.negative && B.negative) {
        subtract_pos(B, A);
      } else if (A.negative) {
        add_pos(A, B);
        negative = true;
      } else if (B.negative) {
        add_pos(A, B);
      } else {
        subtract_pos(A, B);
      }
    }

    void multiply_pos (bInt * A, bInt * B) {	//Multiplies 2 positive big integers
      int carry = 0;
      int max = A->size() + B->size() - 1;
      for (int h = 0; h < max; h++) {
        int sum = 0;
        for (int i = h, j = 0; i >= 0; i--, j++) {
          sum += A->digit(i) * B->digit(j);
        }
        sum += carry;
        carry = sum / 10;
        push_back(sum % 10);
      }
      while (carry > 0) {
        push_back(carry % 10);
        carry /= 10;
      }
    }

    void multiply (bInt * A, bInt * B) {	//Multiplies 2 big integers
      clear();
      if (A->negative && B->negative) {
        negative = false;
      } else if (A->negative || B->negative) {
        negative = true;
      } else {
        negative = false;
      }
      multiply_pos(A, B);
    }

    void factorial (bInt * A) {			//Finds the factorial for a given big integer
      bInt B = *A;
      clear();
      B.decrement();
      multiply_pos(A, &B);
      while (!B.isOne()) {
        B.decrement();
        bInt C = *this;
        multiply(&C, &B);
      }
    }
};

bool notInt (string s) {			//Returns true if the string s cannot be converted to an int
  for (int i = 0; i < s.length(); i++) {
    if (s[0] == '+' || s[0] == '-') {
      continue;
    }
    if (s[i] < '0' || s[i] > '9') {
      return true;
    }
  }
  return false;
}

int properOp(string s) {		//Returns the code for the given operator -1 = invalid, 0 = +, 1 = -, 2 = *, 3 = !
  if (s == "+") {
    return 0;
  } else if (s == "-") {
    return 1;
  } else if (s == "*") {
    return 2;
  } else if (s == "!") {
    return 3;
  } else {
    return -1;
  }
}

void integerHandler(bInt * A) {		//Handles the input of integers
  cout << "Please enter an integer\n";
  string s;
  cin >> s;
  if (!notInt(s)) {
    A->newNum(s);
  } else {
    cout << "Invalid entry\n";
    return integerHandler(A);
  }
}

int main () {
  bInt * A, * B, * C;
  A = new bInt;
  B = new bInt;
  C = new bInt;

  while (true) {
    integerHandler(A);

    //Operation Handler

    cout << "Please enter a valid operation (+,-,*,!)\n";
    string s;
    cin >> s;
    int opNum;
    if (properOp(s) != -1) {
      opNum = properOp(s);
    } else {
      cout << "Invalid entry\n";
      continue;
    }

    //Special Case for Factorial

    if (opNum == 3 && A->negative == false) {
      cout << "The result of the factorial was: "; 
      if (A->size() == 1 && (A->at(0) == 0 || A->at(0) == 1)) {
        cout << "1\n";
      } else {
        C->factorial(A);
        C->print();
      }
      continue;
    } else if(opNum == 3 && A->negative == true) {
      cout << "Invalid operation for given integer value\n";
      continue;
    }

    integerHandler(B);

    //Actual math stuff

    if (opNum == 0) {
      C->add(* A,* B);
      cout << "The result of addition was: ";
    } else if (opNum == 1) {
      C->subtract(* A,* B);
      cout << "The result of subtraction was: ";
    } else if (opNum == 2) {
      C->multiply(A, B);
      cout << "The result of multiplication was: ";
    }

    C->print();
  }

  delete A;
  delete B;
  delete C;

  return 0;
}
