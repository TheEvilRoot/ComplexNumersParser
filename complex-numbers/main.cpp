//
//  main.cpp
//  complex-numbers
//
//  Created by Доктор Кларик on 07/12/2018.
//  Copyright © 2018 Доктор Кларик. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <cmath>

#define STATE_I 0
#define STATE_IM 1
#define STATE_REAL 2
#define STATE_DONE 3

class ComplexNumber {
private:
    std::string stringValue;
    
    double partReal = 0;
    double partImaginary = 0;
    
    bool realSign;
    bool imaginarySign;
    
    bool isSucceed;
    
    std::string error;
    
public:
    ComplexNumber(std::string numberString) {
        this->stringValue = numberString;
        this->isSucceed = false;
    }
    
    ~ComplexNumber() {
        this->stringValue.shrink_to_fit();
    }
    
    std::string getStringValue() {
        return this->stringValue;
    }
    
    double getRealPart(bool isSigned) {
        if (isSigned)
            return this->partReal;
        else return std::fabs(this->partReal);
    }
    
    double getRealPart() {
        return this->getRealPart(true);
    }
    
    double getImaginaryPart(bool isSigned) {
        if (isSigned)
            return this->partImaginary;
        else return std::fabs(this->partImaginary);
    }
    
    double getImaginaryPart() {
        return this->getImaginaryPart(true);
    }
    
    bool getRealPartSign() {
        return this->realSign;
    }
    
    bool getImaginaryPartSign() {
        return this->imaginarySign;
    }
    
    char getRealPartSign(bool showPlus) {
        return this->realSign ? (showPlus ? '+' : '\0') : '-';
    }
    
    char getImaginaryPartSign(bool showPlus) {
        return this->imaginarySign ? (showPlus ? '+' : '\0') : '-';
    }
    
    bool isBuildSuccess() {
        return isSucceed;
    }
    
    std::string getErrorMessage() {
        return error;
    }
    
    void build() {
        if (isSucceed) return;
        
        short int state = STATE_I;
        
        double power = 0.0;
        
        for (long i = stringValue.length() - 1; i >= 0; i--) {
            
            char cur = stringValue[i];
            
            switch(state) {
                case STATE_I: {
                    
                    if (cur != 'i') {
                        error = "Last character is not a 'i'";
                        return;
                    }
                    if (i == 0) {
                        realSign = true;
                        partReal = 0;
                        partImaginary = 1;
                        imaginarySign = true;
                        state = STATE_DONE;
                    } else state = STATE_IM;
                    break;
                }
                case STATE_IM: {
                    if (cur >= '0' && cur <= '9') {
                        partImaginary += ((cur - '0') * pow(10.0, power++));
                    } else if (cur == '-' || cur == '+') {
                        if (partImaginary == 0) partImaginary = 1;
                        partImaginary *= pow(-1.0, (cur == '+') - 1);
                        imaginarySign = cur == '+';
                        state = STATE_REAL;
                        power = 0;
                    } else {
                        error = "Unexpected characted detected in imaginary part";
                        return;
                    }
                    if (i == 0) {
                        realSign = true;
                        partReal = 0;
                        state = STATE_DONE;
                    }
                    break;
                }
                case STATE_REAL: {
                    if (cur >= '0' && cur <= '9') {
                        partReal += ((cur - '0') * pow(10.0, power++));
                        if (i == 0) {
                            state = STATE_DONE;
                            power = 0;
                            realSign = true;
                        }
                    } else if (cur == '-' || cur == '+') {
                        partReal *= pow(-1.0, (cur == '+') - 1);
                        realSign = cur == '+';
                        state = STATE_DONE;
                    } else {
                        error = "Unexpected characted detected in real part";
                        return;
                    }
                    break;
                }
                case STATE_DONE: {
                    isSucceed = true;
                    return;
                }
            }
        }
        
        if (state == STATE_DONE) {
            isSucceed = true;
            return;
        } else if (state == STATE_IM){
            error = "Imaginary end of stream";
            return;
        } else if (state == STATE_REAL) {
            error = "Real end of stream";
            return;
        } else if (state == STATE_I) {
            error = "I end of stream";
            return;
        } else {
            error = "Unexpected error";
            return;
        }
    }
    
    void assert(double realExpected, double imaginaryExpected, bool successExpected) {
        
        if (!isSucceed && successExpected) {
            std::cout << "[CN:ASSERT] Build error!" << std::endl << getErrorMessage() << std::endl;
            return;
        }
        
        if (getImaginaryPart() != imaginaryExpected) {
            std::cout << "[CN:ASSERT] Imaginary part assertion error: " << std::endl << "Expected: " << imaginaryExpected << std::endl << "Actual: " << getImaginaryPart() << std::endl;
            return;
        }
        
        if (getRealPart() != realExpected) {
            std::cout << "[CN:ASSERT] Real part assertion error: " << std::endl << "Expected: " << realExpected << std::endl << "Actual: " << getRealPart() << std::endl;
            return;
        }
        
        std::cout << "[CN:ASSERT] Passed for " << getStringValue() << std::endl;
    }
};


// Tests

int main(int argc, const char * argv[]) {
    ComplexNumber numbers[12] = {
        ComplexNumber("-i"),
        ComplexNumber("i"),
        ComplexNumber("1+i"),
        ComplexNumber("2-i"),
        ComplexNumber("3i"),
        ComplexNumber("-3i"),
        ComplexNumber("-12+i"),
        ComplexNumber("-22-2i"),
        ComplexNumber("-222i"),
        ComplexNumber("1-222i"),
        ComplexNumber("qi"),
        ComplexNumber("123+2")
    };

    double realResults[12] {
        0,
        0,
        1,
        2,
        0,
        0,
        -12,
        -22,
        0,
        1,
        0,
        0
    };
    
    double imaginaryResults[12] = {
        -1,
        1,
        1,
        -1,
        3,
        -3,
        1,
        -2,
        -222,
        -222,
        0,
        0
    };
    
    
    for (int i = 0; i < 12; i++) {
        numbers[i].build();
        numbers[i].assert(realResults[i], imaginaryResults[i], imaginaryResults[i] != 0);
    }
    
    
    return 0;
}
