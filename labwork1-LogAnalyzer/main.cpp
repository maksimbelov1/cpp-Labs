#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <string>


const int16_t kBufferLen = 4096;
const int16_t kBufferLenStr = 1024;

struct Arguments {
    char input_name[256];
    bool is_input_name = false;
    char output_name[256];
    bool is_output_name = false;
    bool is_print = false;
    uint32_t n_stats = 0;
    uint32_t window_interval = 0;
    uint32_t min_time = 0;
    uint32_t max_time = UINT32_MAX;
    bool is_window = false;
    bool input_error = false;
    bool open_file_error = false;
    FILE* input_file = nullptr;
    FILE* output_file = nullptr;

};

Arguments Parser(int argc, char** argv) {
    uint8_t kStats = 10;
    Arguments result;

    for (int i = 1; i < argc; ++i) {

        if (strncmp(argv[i], "--output=",  sizeof("--output=") - 1) == 0) {
            strncpy(result.output_name, argv[i] + 9, sizeof(result.output_name) - 1);
            result.is_output_name = true;
        } else if ((strncmp(argv[i], "-o", sizeof("-o") - 1) == 0) || (strncmp(argv[i], "--output",  sizeof("--output") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            strncpy(result.output_name, argv[++i], sizeof(result.output_name) - 1);
            result.is_output_name = true;
        } else if (strncmp(argv[i], "--print",  sizeof("--print") - 1) == 0) {
            result.is_print = true;
        } else if (strncmp(argv[i], "-p",  sizeof("-p") - 1) == 0) {
            result.is_print = true;
        } else if (strncmp(argv[i], "--stats=",  sizeof("--stats=") - 1) == 0) {
            result.n_stats = std::stoi(argv[i] + 8);
        } else if ((strncmp(argv[i], "-s",  sizeof("-s") - 1) == 0) || (strncmp(argv[i], "--stats", sizeof("--stats") - 1) == 0)) {
            if (std::isdigit(argv[i + 1][0])) {
                result.n_stats = std::stoi(argv[++i]);
            } else {
                result.n_stats = kStats;
            }
        } else if (strncmp(argv[i], "--window=",  sizeof("--window=") - 1) == 0) {
            result.window_interval = std::stoi(argv[i] + 9);
            result.is_window = true;
        } else if ((strncmp(argv[i], "-w",  sizeof("-w") - 1) == 0) || (strncmp(argv[i], "--window",  sizeof("--window") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            result.window_interval = std::stoi(argv[++i]);
            result.is_window = true;
        } else if (strncmp(argv[i], "--from=",  sizeof("--from=") - 1) == 0) {
            result.min_time = std::stoi(argv[i] + 7);
        } else if ((strncmp(argv[i], "-f",  sizeof("-f") - 1) == 0) || (strncmp(argv[i], "--from",  sizeof("--from") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            result.min_time = std::stoi(argv[++i]);
        } else if (strncmp(argv[i], "--to=",  sizeof("--to=") - 1) == 0) {
            result.max_time = std::stoi(argv[i] + 5);
        } else if ((strncmp(argv[i], "-t",  sizeof("-t") - 1) == 0) || (strncmp(argv[i], "--to",  sizeof("--to") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            result.max_time = std::stoi(argv[++i]);
        } else {
            strncpy(result.input_name, argv[i], sizeof(result.input_name) - 1);
            result.is_input_name = true;
            }
    }
    if (!result.is_input_name) {
        std::cout << "Input file missing";
    } else {
        result.input_file = fopen(result.input_name, "r");
        if (!result.input_file) {
            std::cerr << "Cannot open input file";
            result.open_file_error = true;
        }
    }

    if (result.is_output_name) {
        result.output_file = fopen(result.output_name, "w");
        if (!result.output_file) {
            std::cerr << "Cannot open output file";
            result.open_file_error = true;
        }
    }
    return result;
}


int TmToTimestamp(char* year, char* month, char* day, char* hour, char* minute, char* seconds, char* time_zone) {
    int16_t time_zone_int=std:: stoi(time_zone);
    uint16_t year_int=std:: stoi(year);
    uint16_t day_int=std:: stoi(day);
    uint16_t hour_int=std:: stoi(hour);
    uint16_t minute_int=std:: stoi(minute);
    uint16_t seconds_int=std:: stoi(seconds);
    uint8_t month_int;
    const char mon[][13] = {"Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug" "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i) {
        if (strncmp(month, mon[i], 3) == 0) {
            month_int = i;
        }
    }
    std::tm date;
    uint8_t my_time_zone = 3;
    date.tm_year = year_int - 1900;
    date.tm_mon = month_int;
    date.tm_mday = day_int;
    date.tm_hour = hour_int - my_time_zone + time_zone_int;
    date.tm_min = minute_int;
    date.tm_sec = seconds_int;
    std::time_t timestamp = std::mktime(&date);
    return timestamp;
}
 


void AnalyzeAndPrintStats(int error_count, int n_stats, char* output_name) {
    FILE* output_file = fopen(output_name, "r");
    if (!output_file) {
        std::cerr << "Cannot open file:" << output_name;
        return;
    }
    char** requests = new char*[error_count];
    for (int i = 0; i < error_count; ++i) {
        requests[i] = new char[256];
    }
    int* requests_counts = new int[error_count];
    for (int i = 0; i < error_count; ++i) {
        requests_counts[i] = 1;
    }
    char buffer[kBufferLen];
    char buffer_str[kBufferLenStr];
    uint32_t step_buffer_str = 0;
    uint32_t last_flag = 0;
    uint32_t first_flag = 0;
    uint32_t step_requests = 0;
    char error_requests[256];

    while (!std::feof(output_file)) {
        size_t read_lenght = fread(buffer, 1, kBufferLen, output_file);
        
        for (int i = 0; i < read_lenght; ++i) {
            if ((i == read_lenght-1) && (buffer[i] != '\n')) {
                fseek(output_file, -step_buffer_str,SEEK_CUR);
                step_buffer_str = 0;
                buffer_str[0] = '\0';
                break;
            }
            if ((buffer[i] != '\n') && (buffer[i] != '\0')) {
                buffer_str[step_buffer_str] = buffer[i];
                ++step_buffer_str;
            }
            else{
                buffer_str[step_buffer_str] = '\0';
                first_flag = 0;
                last_flag = 0;
                for (int j = 0; j < step_buffer_str - 1; ++j) {
                    if ((buffer_str[j] == '"') && (first_flag == 0)) {
                        first_flag = j;
                    }
                    if (buffer_str[j] == '"') {
                        last_flag = j;
                    }
                }
                buffer_str[last_flag] = '\0';
                uint32_t step_error_requests = 0;
                for (int j = first_flag+1; j <= last_flag; ++j) {
                    requests[step_requests][step_error_requests] = buffer_str[j];
                    ++step_error_requests;
                }
                ++step_requests;
                buffer_str[0] = '\0';
                step_buffer_str = 0;  
            }
        }
    }
    for (int i = 0; i < error_count; ++i) {    
        for (int j = i + 1; j < error_count - 1; ++j) {
            if ((strcmp(requests[j], requests[i]) == 0) && (requests[j][0] != '%') && (requests[i][0] != '%')) {
                ++requests_counts[i];
                requests_counts[j] = 0;
                requests[j][0] = '%';
            }
        }
    }
    uint32_t CountUniqueRequests = 0;
    for (int i = 0; i < error_count; ++i) {
        if (requests_counts[i]) {
            ++CountUniqueRequests;
        }
    }
    if (n_stats > CountUniqueRequests ) {
        std::cout << "stats=" << n_stats << " > numbers or unique requests with ststus code 5XX" << std::endl << "will be output stats=" << CountUniqueRequests<< std::endl<<std::endl;
        n_stats = CountUniqueRequests;
    }
    uint32_t max_count = 0;
    uint32_t max_count_index = 0;
    std::cout << "Top " << n_stats << " requests with status code 5XX:" << std::endl; 
    for (int i = 0; i < n_stats; ++i) {
        for (int j = 0; j < error_count; ++j) {
            if (requests_counts[j] > max_count) {
                max_count = requests_counts[j];
                max_count_index = j;
            }
        }
        std::cout << requests[max_count_index] << std::endl;
        requests_counts[max_count_index] = 0;
        max_count = 0;
        max_count_index = 0;
    }
    delete [] requests_counts;
    delete [] requests;
}




void AnalyzeWindow(uint32_t* window_request_count, uint64_t timestamp, uint64_t& last_window_time, uint32_t window_interval, uint32_t& current_count_window, uint32_t& max_count_window, uint32_t& max_count_window_index) {
    if (timestamp == last_window_time) {
        ++window_request_count[window_interval - 1];
        ++current_count_window;
    } else if (timestamp - last_window_time >= window_interval) {
        for (int k = 0; k < window_interval - 1; ++k) {
            window_request_count[k] = 0;
        }
        window_request_count[window_interval - 1] = 1;
        last_window_time = timestamp;
        current_count_window = 1;
    } else {
        for (int k = 0; k < timestamp - last_window_time; ++k) {
            current_count_window -= window_request_count[k];
        }
        ++current_count_window; 
        for (int k = 0; k < window_interval; ++k) {
            if (k + timestamp - last_window_time < window_interval) {
                window_request_count[k] = window_request_count[k + (timestamp - last_window_time )];
            } else {
                    window_request_count[k] = 0;
            }
        }
        ++window_request_count[window_interval - 1];
        last_window_time = timestamp;
    }
    if (max_count_window < current_count_window) {
        max_count_window = current_count_window;
        max_count_window_index = last_window_time;
    }
}



int main(int argc, char** argv) {
    Arguments paresed_arguments = Parser(argc, argv); 
    FILE* input_file = paresed_arguments.input_file;
    FILE* output_file = paresed_arguments.output_file;;
    uint32_t window_interval = paresed_arguments.window_interval;
    if (!paresed_arguments.is_input_name) {
        EXIT_SUCCESS;
    }
    if (paresed_arguments.open_file_error) {
        EXIT_FAILURE;
    }
    uint32_t* window_request_count = new uint32_t[window_interval];
    for (int i = 0; i < window_interval; ++i) {
        window_request_count[i] = 0;
    }
    uint64_t last_window_time = 0;
    uint32_t current_count_window = 0;
    uint32_t max_count_window = 0;
    uint32_t max_count_window_index = 0;
    uint32_t first_time = 0;
    uint32_t last_flag = 0;
    bool is_analyze = false;
    uint32_t step_buffer_str = 0;
    char buffer[kBufferLen];
    char buffer_str[kBufferLenStr];
    uint32_t error_count = 0;
    uint16_t lower_error_status = 500;
    uint16_t upper_error_status = 599;

    while (!std::feof(input_file)) {
        size_t read_lenght = fread(buffer, 1, kBufferLen, input_file);
        
        for (int i = 0; i < read_lenght; ++i) {
            if ((i == read_lenght - 1) && (buffer[i] != '\n')) {
                fseek(input_file, -step_buffer_str, SEEK_CUR);
                step_buffer_str = 0;
                last_flag = 0;
                break;
            }
            
            if ((buffer[i] != '\n') && (buffer[i] != '\0')) {
                buffer_str[step_buffer_str] = buffer[i];
                ++step_buffer_str; 
            }
            else {
                for (int j = 0; j < step_buffer_str; ++j) {
                    if (buffer_str[j] == '"') {
                        last_flag = j;
                    }
                    if (buffer_str[j] == '[') {
                        char year[8];
                        char month[4];
                        char day[8];
                        char hour[8];
                        char minute[8];
                        char seconds[8];
                        char time_zone[8];
                        day[0] = buffer_str[j + 1];
                        day[1] = buffer_str[j + 2];
                        month[0] = buffer_str[j + 4];
                        month[1] = buffer_str[j + 5];
                        month[2] = buffer_str[j + 6];
                        year[0] = buffer_str[j + 8];
                        year[1] = buffer_str[j + 9];
                        year[2] = buffer_str[j + 10];
                        year[3] = buffer_str[j + 11];
                        hour[0] = buffer_str[j + 13];
                        hour[1] = buffer_str[j + 14];
                        minute[0] = buffer_str[j + 16];
                        minute[1] = buffer_str[j + 17];
                        seconds[0] = buffer_str[j + 19];
                        seconds[1] = buffer_str[j + 20];
                        time_zone[0] = buffer_str[j + 22];
                        time_zone[1] = buffer_str[j + 23];
                        time_zone[2] = buffer_str[j + 24];
                
                        uint32_t timestamp = TmToTimestamp(year, month, day, hour, minute, seconds, time_zone);
                        if (first_time == 0) {
                            first_time = timestamp;
                        }
                        if ((timestamp >= paresed_arguments.min_time) && (timestamp <= paresed_arguments.max_time)) {
                            is_analyze = true;
                            if (paresed_arguments.is_window) {
                                AnalyzeWindow(window_request_count, timestamp, last_window_time, window_interval, current_count_window, max_count_window, max_count_window_index);
                            }
                        }
                    }
                }
                if (is_analyze) {
                    char server_code[4];
                    server_code[0] = buffer_str[last_flag + 2];
                    server_code[1] = buffer_str[last_flag + 3];
                    server_code[2] = buffer_str[last_flag + 4];
                    int16_t server_code_int = std::stoi(server_code);
                    if ((server_code_int >= lower_error_status) && (server_code_int <= upper_error_status)) {
                        buffer_str[step_buffer_str] = '\n';
                        if (paresed_arguments.is_output_name) {
                            fwrite(buffer_str, 1, step_buffer_str + 1, output_file);
                        }
                        if (paresed_arguments.is_print) {
                            buffer_str[step_buffer_str] = '\0';
                            std::cout << buffer_str << std::endl;
                        }
                        ++error_count;
                    }
                }
                buffer_str[0] = '\0';
                step_buffer_str = 0;
                last_flag = 0;
                is_analyze = false;
            }         
        }
    }

    if (paresed_arguments.is_window) {
        time_t end_wint_t = static_cast<time_t>(max_count_window_index);
        time_t start_wint_t = static_cast<time_t>(max_count_window_index - window_interval + 1);
        std:: cout << "Window with maximum nimbers or requests: ";
        std::cout << std::put_time(std::gmtime(&start_wint_t), "%Y-%m-%d-%H:%M:%S %z%") << " - - ";
        std::cout << std::put_time(std::gmtime(&end_wint_t), "%Y-%m-%d-%H:%M:%S %z%") << std::endl;
    }
    fclose(input_file);
    fclose(output_file);

    if (paresed_arguments.n_stats && paresed_arguments.is_output_name) {
        AnalyzeAndPrintStats(error_count, paresed_arguments.n_stats, paresed_arguments.output_name);
    }
    if (paresed_arguments.n_stats && (!paresed_arguments.is_output_name)) {
        std::cout << "To analyze requests with errors, enter the output file name";
    }
}