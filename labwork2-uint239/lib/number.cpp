#include "number.h"
#include <cmath>
#include <cstring> 



uint239_t FromInt(uint32_t value, uint64_t shift) {
    uint239_t final_number = {0};
    uint16_t step = 0;
    uint64_t meaningful_shift = shift % 245;
    while (value > 0){
        if (value % 2) {
            if ((244 - step - meaningful_shift) > 0) {
                final_number.data[(244 - step - meaningful_shift) / 7] |= (1 << (6 - ((244 - step - meaningful_shift) % 7)));
            } else {
                final_number.data[(step + meaningful_shift) / 7] |= (1 << (6 - ((step + meaningful_shift) % 7)));
            } 
        }
        value /= 2;
        ++step;
    }

    for (int i = sizeof(final_number) - 1; i >= 0; --i) {
        if (shift % 2) {
            final_number.data[i] |= (1 << 7);
        }
        shift /= 2;
    }
    return final_number;
}


uint239_t FromString(const char* str, uint32_t shift) {
    char digits[245];
    strcpy(digits, str);
    uint239_t final_number = {0};
    int32_t meaningful_shift = shift % 245;
    uint8_t remainder = 0;
    uint16_t current_number = 0;
    for (int i = 0; i < 245; ++i) {
        remainder = 0;
        current_number = 0;
        for (int j = 0; j < strlen(str); ++j) {
            current_number = 10 * remainder + (digits[j] - '0');
            digits[j] = (current_number / 2) + '0';
            remainder = current_number % 2;
        }
        if (remainder) {
            final_number.data[((245 - i - meaningful_shift + 244) % 245) / 7] |= (1 << (6 - (((245 - i - meaningful_shift + 244) % 245) % 7)));

        }
    }
    for (int i = sizeof(final_number) - 1; i >= 0; --i) {
        if (shift % 2) {
            final_number.data[i] |= (1 << 7);
        }
        shift /= 2;
    }

    return final_number;
}


uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t final_number = {0};
    uint64_t shift_lhs = 0;
    uint64_t shift_rhs = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    int64_t shift_sum = 0;
    uint64_t meaningful_shift_lhs = shift_lhs % 245;
    uint64_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask = 0;
    uint8_t first_temp = 0;
    uint8_t second_temp = 0;
    uint8_t remainder = 0;

    if (shift_lhs >= std::pow(2, 35) - shift_rhs) {
        shift_sum = shift_lhs - std::pow(2, 35) + shift_rhs;
    } else {
        shift_sum = shift_lhs + shift_rhs;
    }
    uint64_t meaningful_shift_sum = shift_sum % 245;

    for (int i = 244; i >= 0; --i) {
        if (i >= meaningful_shift_lhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_lhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i + 245 - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_lhs) % 7));
        }

        if (i >= meaningful_shift_rhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i + 245 - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_rhs) % 7));
        }

      

        if (first_temp + second_temp + remainder == 3) {
            final_number.data[((i + 245 - meaningful_shift_sum) % 245) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_sum) % 245) % 7));
            remainder = 1;
        } else if (first_temp + second_temp + remainder == 2) {
            remainder = 1;
        } else if (first_temp + second_temp + remainder == 1) {
            final_number.data[((i + 245 - meaningful_shift_sum) % 245) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_sum) % 245) % 7));
            remainder = 0;
        } else {
            remainder = 0;
        }
    }

    for (int i = sizeof(final_number) - 1; i >= 0; --i) {
        if (shift_sum % 2) {
            final_number.data[i] |= (1 << 7);
        }
        shift_sum /= 2;
    }
    return uint239_t(final_number);
}




uint239_t operator+=(uint239_t& lhs, const uint239_t& rhs) {
    uint64_t shift_lhs = 0;
    uint64_t shift_rhs = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    int64_t shift_sum = 0;
    uint64_t meaningful_shift_lhs = shift_lhs % 245;
    uint64_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask = 0;
    uint8_t first_temp = 0;
    uint8_t second_temp = 0;
    uint8_t remainder = 0;

    if (shift_lhs >= std::pow(2, 35) - shift_rhs) {
        shift_sum = shift_lhs - std::pow(2, 35) + shift_rhs;
    } else {
        shift_sum = shift_lhs + shift_rhs;
    }
    uint64_t meaningful_shift_sum = shift_sum % 245;

    for (int i = 244; i >= 0; --i) {
        if (i >= meaningful_shift_lhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_lhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i + 245 - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_lhs) % 7));
        }
        if (i >= meaningful_shift_rhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i + 245 - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_rhs) % 7));
        }

      

        if (first_temp + second_temp + remainder == 3) {
            if (i >= meaningful_shift_lhs) {
                lhs.data[(i - meaningful_shift_lhs) / 7] |= (1 << (6 - (i - meaningful_shift_lhs) % 7));
            } else {
                lhs.data[((i + 245 - meaningful_shift_lhs)) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_lhs)) % 7));
            }
            remainder = 1;
        } else if (first_temp + second_temp + remainder == 2) {
            if (i >= meaningful_shift_lhs) {
                lhs.data[(i - meaningful_shift_lhs) / 7] &= ~(1 << (6 - (i - meaningful_shift_lhs) % 7));
            } else {
                lhs.data[((i + 245 - meaningful_shift_lhs)) / 7] &= ~(1 << (6 - ((i + 245 - meaningful_shift_lhs)) % 7));
            }
            remainder = 1;
        } else if (first_temp + second_temp + remainder == 1) {
            if (i >= meaningful_shift_lhs) {
                lhs.data[(i - meaningful_shift_lhs) / 7] |= (1 << (6 - (i - meaningful_shift_lhs) % 7));
            } else {
                lhs.data[((i + 245 - meaningful_shift_lhs)) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_lhs)) % 7));
            }
            remainder = 0;
        } else {
            if (i >= meaningful_shift_lhs) {
                lhs.data[(i - meaningful_shift_lhs) / 7] &= ~(1 << (6 - (i - meaningful_shift_lhs) % 7));
            } else {
                lhs.data[((i + 245 - meaningful_shift_lhs)) / 7] &= ~(1 << (6 - ((i + 245 - meaningful_shift_lhs)) % 7));
            }
            remainder = 0;
        }
    }

    return lhs;
}



uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t final_number = {0};
    uint64_t shift_lhs = 0;
    uint64_t shift_rhs = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    int64_t shift_sum = 0;
    uint64_t meaningful_shift_lhs = shift_lhs % 245;
    uint64_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask = 0;
    uint8_t first_temp = 0;
    uint8_t second_temp = 0;
    uint8_t remainder = 0;

    if (shift_lhs < shift_rhs) {
        shift_sum = std::pow(2, 35) - shift_rhs + shift_lhs;
    } else {
        shift_sum = shift_lhs - shift_rhs;
    }
    uint64_t meaningful_shift_sum = shift_sum % 245;

    for (int i = 244; i >= 0; --i) {
        if (i >= meaningful_shift_lhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_lhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i + 245 - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_lhs) % 7));
        }

        if (i >= meaningful_shift_rhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i + 245 - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_rhs) % 7));
        }

      
        if (first_temp - second_temp - remainder == -2) {
            remainder = 1;
        } else if (first_temp - second_temp - remainder == -1) {
            final_number.data[((i + 245 - meaningful_shift_sum) % 245) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_sum) % 245) % 7));
            remainder = 1;
        } else if (first_temp - second_temp - remainder == 0) {
            remainder = 0;
        } else {
            final_number.data[((i + 245 - meaningful_shift_sum) % 245) / 7] |= (1 << (6 - ((i + 245 - meaningful_shift_sum) % 245) % 7));
            remainder = 0;
        }
    }
    for (int i = sizeof(final_number) - 1; i >= 0; --i) {
        if (shift_sum % 2) {
            final_number.data[i] |= (1 << 7);
        }
        shift_sum /= 2;
    }
    return uint239_t(final_number);
}



uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t final_number = {0};
    uint32_t shift_lhs = 0;
    uint32_t shift_rhs = 0;
    int64_t shift_sum = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    uint16_t meaningful_shift_lhs = shift_lhs % 245;
    uint16_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask;
    uint8_t factor = 0;
    uint239_t temp = lhs;
    uint64_t new_shift = 0;
    uint16_t step = 0;

    if (shift_lhs > std::pow(2, 35) - shift_rhs) {
        shift_sum = shift_lhs - std::pow(2, 35) + shift_rhs;
    } else {
        shift_sum = shift_lhs + shift_rhs;
    }
    uint64_t meaningful_shift_sum = shift_sum % 245;

    for (int i = 244; i >= 0; --i) {
        if (i >= meaningful_shift_rhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            factor = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i + 245 - meaningful_shift_rhs) % 7));
            factor = (rhs.data[(i + 245 - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i + 245 - meaningful_shift_rhs) % 7));
        }

        if (shift_lhs >= step) {
            new_shift = shift_lhs - step;
        }
        else {
            new_shift = shift_lhs + 245 - step;
        }
        if (factor) {
            for (int j = sizeof(final_number) - 1; j >= 0; --j) {
                if (temp.data[j] >= 128) {
                    temp.data[j] -= 128;
                }
                temp.data[j] |= ((new_shift % 2) << 7);
                new_shift /= 2;
            }
            final_number = final_number + temp;
            final_number = final_number - FromInt(0, GetShift(temp));
        }
        ++step;
    }
    final_number = final_number + FromInt(0, shift_sum);
    return uint239_t(final_number);
}

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs) {
    uint239_t numerator = lhs;
    uint239_t final_number = {0};
    uint64_t shift_lhs = 0;
    uint64_t shift_rhs = 0;
    uint64_t shift_sum = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    uint64_t meaningful_shift_lhs = shift_lhs % 245;
    uint64_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask = 0;
    uint8_t first_temp = 0;
    uint8_t second_temp = 0;
    int32_t step = -1;
    uint239_t temp = rhs;
    uint32_t temp_shift = 0;

    if (shift_lhs < shift_rhs) {
        shift_sum = std::pow(2, 35) - shift_rhs + shift_lhs;
    } else {
        shift_sum = shift_lhs - shift_rhs;
    }
    int32_t max_shift = -1;

    for (int i = 244; i >= 0; --i) {
        if (i >= meaningful_shift_rhs) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs + 245) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs + 245) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs + 245) % 7));
        }
        if (second_temp) {
            max_shift = i;
        }
    } 

    if (max_shift == -1) {
        std::cerr << "You can't divide by 0!" << std::endl;
        return final_number;
    }

    bool is_divisible = true;
    for (int i = max_shift - 1; i >= 0; --i) {
        is_divisible = true;
        for (int j = 244; j >= 0; --j) {
            bit_mask = 1 << (6 - (((j + 245 - meaningful_shift_lhs)% 245) % 7));
            first_temp = (numerator.data[((j + 245 - meaningful_shift_lhs)% 245) / 7] & bit_mask) >>  (6 - (((j + 245 - meaningful_shift_lhs) % 245) % 7));
        
            bit_mask = 1 << (6 - (((j + 245 - meaningful_shift_rhs + i) % 245) % 7));
            second_temp = (rhs.data[((j + 245 - meaningful_shift_rhs + i) % 245) / 7] & bit_mask) >>  (6 - (((j + 245 - meaningful_shift_rhs + i) % 245) % 7));
        
            if (first_temp > second_temp) {
                is_divisible = true;
            } else if (first_temp < second_temp) {
                is_divisible = false;
            }
        }
     
        if (is_divisible) {
            if (step == -1) {
                step = 0;
            }
            temp_shift = (245 + meaningful_shift_rhs - i) % 245;
            numerator = numerator + FromInt(0, temp_shift);
            for (int k = sizeof(temp) - 1; k >= 0; --k) {
                if (temp.data[k] >= 128) {
                    temp.data[k] -= 128;
                }
                if (temp_shift % 2) {
                    temp.data[k] |= (1 << 7);
                }
                temp_shift /= 2;
            }
            numerator = numerator - temp;
            final_number.data[step / 7] |= (1 << (6 - (step % 7)));
        } 
        if (step != -1) {
            ++step;
        }
    }
   
    temp_shift = 245 - step;
    for (int i = sizeof(final_number) - 1; i >= 0; --i) {
        if (temp_shift % 2) {
            final_number.data[i] |= (1 << 7);
        }
        temp_shift /= 2;
    }
    final_number = final_number - FromInt(0, 245 - step);
    final_number = final_number + FromInt(0, shift_sum);
    return uint239_t(final_number);
}


bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    uint32_t shift_lhs = 0;
    uint32_t shift_rhs = 0;
    shift_lhs = GetShift(lhs);
    shift_rhs = GetShift(rhs);
    uint16_t meaningful_shift_lhs = shift_lhs % 245;
    uint16_t meaningful_shift_rhs = shift_rhs % 245;
    uint8_t bit_mask;
    uint8_t first_temp = 0;
    uint8_t second_temp = 0;

    for (int i = 244; i >= 0; --i) {
        if (i - meaningful_shift_lhs >= 0) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_lhs) % 7));
            first_temp = (lhs.data[(i - meaningful_shift_lhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_lhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i - meaningful_shift_lhs + 245) % 7));
            first_temp = (lhs.data[(i - meaningful_shift_lhs + 245) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_lhs + 245) % 7));
        }

        if (i - meaningful_shift_rhs >= 0) {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs) % 7));
        } else {
            bit_mask = 1 << (6 - ((i - meaningful_shift_rhs + 245) % 7));
            second_temp = (rhs.data[(i - meaningful_shift_rhs + 245) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift_rhs + 245) % 7));
        }

        if (first_temp != second_temp) {
            return false;
            break;
        }
    }
    return true;
}


bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    return !(operator==(lhs, rhs));
}


std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
    uint64_t shift = 0;
    uint64_t meaningful_shift = 0;
    shift = GetShift(value);
    meaningful_shift = shift % 245;
    uint8_t decimal_value[80] = {0};
    uint8_t big_degree[80] = {0};
    uint32_t first_symbol = 79;
    uint32_t current_number = 0;
    uint8_t remainder = 0;
    uint8_t bit_value = 0;
    uint8_t bit_mask = 0;
   
    for (int i = 244; i >= 0; --i) {
        for (int j = 79; j >= first_symbol; --j) {
            if ((big_degree[j] * 2 + remainder) > 9) {
                big_degree[j] = (big_degree[j] * 2 + remainder) % 10;
                remainder = 1;
            } else {
                big_degree[j] = big_degree[j] * 2 + remainder;
                remainder = 0;
            }
        }
        if (big_degree[79] == 0) {
            big_degree[79] = 1;
        }
        if (remainder) {
            big_degree[--first_symbol] = 1;
        }

        if (i >= meaningful_shift) {
            bit_mask = 1 << (6 - ((i - meaningful_shift) % 7));
            bit_value = (value.data[(i - meaningful_shift) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift) % 7));
        } else {
            bit_mask = 1 << (6 - ((i - meaningful_shift + 245) % 7));
            bit_value = (value.data[(i - meaningful_shift + 245) / 7] & bit_mask) >>  (6 - ((i - meaningful_shift + 245) % 7));
        }
        
        remainder = 0;
        if (bit_value) {
            for (int j = 79; j >= first_symbol - 1; --j) {
                if (big_degree[j] + decimal_value[j] + remainder > 9) {
                    decimal_value[j] = (big_degree[j] + decimal_value[j] + remainder) % 10;
                    remainder = 1;
                } else {
                    decimal_value[j] = big_degree[j] + decimal_value[j] + remainder;
                    remainder = 0;
                }
            }
        }
    }
    uint16_t step = 0;
    while ((decimal_value[step] == 0) && (step < sizeof(decimal_value))) {
        ++step;
    }
    if (step == sizeof(decimal_value)) {
        stream << '0';
    }
    for (int i = step; i < sizeof(decimal_value); ++i) {
        stream << static_cast<char>(decimal_value[i] + '0');
    }   
    return stream;
}


uint64_t GetShift(const uint239_t& value) {
    uint64_t shift = 0;
    uint64_t degree = 1;
    for (int i = sizeof(value) - 1; i >= 0; --i) {
        if (value.data[i] >= 128) { 
            shift += degree;
        }
    degree = degree * 2;
    }
    return shift;
}
