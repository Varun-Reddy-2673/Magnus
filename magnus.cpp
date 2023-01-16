#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std;

typedef vector <string> (*main_function) (vector < vector <string> > array, int line);
vector <string> master_list;
vector <string> variables;
vector <int> pointers;
vector < vector <string> > holders;
vector <main_function> functions;
int set_length = 12;

string to_str (double in_num) {
    string str = to_string (in_num);
    return str;
}

double to_num (string in_string) {
    double number = std::stod (in_string);
    return number;
}

int to_int (string in_string) {
    stringstream inter (in_string);
    int integer;
    inter >> integer;
    return integer;
}

void terminate (string error, int line) {
    cout << "ERROR (line " + to_string (line + 1) + ") - " + error << endl;
    abort ();
}

int char_type (string unit) {
    int type = 5;
    string char_sets [5] = {"'", "([{", ")]}", "|$=%&+-*/^#<>!", ".0123456789"};
    for (int index = 0; index < 5; index ++) {
        string set = char_sets [index];
        if (set.find (unit) != string::npos) {
            type = index;
        }
    }
    return type;
}

int order (string sign) {
    string sets [12] [3] = {{"|", "$ ", " "}, {"=", " ", " "}, {"+=", "-=", " "}, {"*=", "/=", " "}, {"^=", "#=", " "}, {"%", "&", " "}, {"+", "-", " "}, {"*", "/", " "}, {"^", "#", " "}, {"<", ">", "=="}, {"<=", ">=", "!="}, {"!", " ", " "}};
    int final = 0;
    for (int inner = 0; inner < 12; inner ++) {
        for (int outer = 0; outer < 3; outer ++) {
            if (sign == sets [inner] [outer]) {
                final = inner + 1;
            }
        }
    }
    return final;
}

vector <string> index_unit (string command) {
    string part_1 = "";
    string part_2 = "";
    bool space = false;
    bool mode = false;
    for (int index = 0; index < command.length (); index ++) {
        string unit;
        unit += command [index];
        if (mode) {
            part_2 += unit;
        } else {
            if (unit == " " & space) {
                mode = true;
            } else if (unit != " ") {
                if (! space) {
                    space = true;
                }
                part_1 += unit;
            }
        }
    }
    vector <string> halves;
    halves.push_back (part_1);
    halves.push_back (part_2);
    return halves;
}

vector <string> split (string line, string sep) {
    vector <string> array;
    string current;
    for (int index = 0; index < line.length (); index ++) {
        string unit;
        unit += line [index];
        if (unit == sep) {
            array.push_back (current);
            current = "";
        } else {
            current += unit;
        }
    }
    if (current != "") {
        array.push_back (current);
    }
    return array;
}

vector <string> cui_out (vector < vector <string> > array, int line) {
    if (array [0] [0] != "str") {
        terminate ("type <" + array [0] [0] + "> is not printable", line);
    }
    cout << array [0] [1] << endl;
    vector <string> result = array [0];
    return result;
}

vector <string> cui_in (vector < vector <string> > array, int line) {
    if (array.size () > 0) {
        if (array [0] [0] != "str") {
            terminate ("type <" + array [0] [0] + "> is not printable", line);
        }
        cout << array [0] [1];
    }
    string input;
    cin >> input;
    vector <string> result;
    result.push_back ("str");
    result.push_back (input);
    return result;
}

void init_cui () {
    string new_variables [2] = {"cui.out", "cui.in"};
    main_function new_functions [2] = {cui_out, cui_in};
    int size_holders = holders.size ();
    int size_functions = functions.size ();
    for (int index = 0; index < 2; index ++) {
        variables.push_back (new_variables [index]);
        pointers.push_back (size_holders + index);
        vector <string> value_set;
        value_set.push_back ("func");
        value_set.push_back (to_str (size_functions + index));
        holders.push_back (value_set);
        functions.push_back (new_functions [index]);
    }
}

vector <string> prs_num (vector < vector <string> > array, int line) {
    vector <string> result = array [0];
    if (result [0] == "bool") {
        if (result [1] == "true") {
            result [1] = "1";
        } else if (result [1] == "false") {
            result [1] = "0";
        }
    } else if (result [0] != "num" && result [0] != "str") {
        terminate ("prs.num () does not take type <" + result [0] + "> as a paramater", line);
    }
    result [0] = "num";
    return result;
}

vector <string> prs_str (vector < vector <string> > array, int line) {
    vector <string> result = array [0];
    if (result [0] != "str" && result [0] != "num" && result [0] != "bool") {
        terminate ("prs.str () does not take type <" + result [0] + "> as a paramater", line);
    } else if (result [0] == "num") {
        string output = "";
        int len = result [1].length ();
        vector <string> parts = split (result [1], ".");
        bool blank = parts.size () == 2;
        for (int index = len - 1; index >= 0; index --) {
            string unit;
            unit += result [1] [index];
            if (unit == ".") {
                if (blank) {
                    blank = false;
                } else {
                    output = "." + output;
                }
            } else if (unit == "0") {
                if (! blank) {
                    output = unit + output;
                }
            } else {
                output = unit + output;
                if (blank) {
                    blank = false;
                }
            }
        }
        result [1] = output;
    }
    result [0] = "str";
    return result;
}

vector <string> prs_array (vector < vector <string> > array, int line) {
    vector <string> result;
    if (array [0] [0] == "str") {
        string str = array [0] [1];
        string output = "";
        for (int index = 0; index < str.length (); index ++) {
            string unit;
            unit += str [index];
            int pointer = holders.size ();
            output += " " + to_string (pointer);
            vector <string> value_set;
            value_set.push_back ("str");
            value_set.push_back (unit);
            holders.push_back (value_set);
        }
        if (output != "") {
            output = output.substr (1, output.length () - 1);
        }
        result.push_back ("array");
        result.push_back (output);
    } else if (array [0] [0] == "array") {
        result = array [0];
    }
    return result;
}

void init_prs () {
    string new_variables [3] = {"prs.num", "prs.str", "prs.array"};
    main_function new_functions [3] = {prs_num, prs_str, prs_array};
    int size_holders = holders.size ();
    int size_functions = functions.size ();
    for (int index = 0; index < 3; index ++) {
        variables.push_back (new_variables [index]);
        pointers.push_back (size_holders + index);
        vector <string> value_set;
        value_set.push_back ("func");
        value_set.push_back (to_str (size_functions + index));
        holders.push_back (value_set);
        functions.push_back (new_functions [index]);
    }
}

vector <string> data_type (vector < vector <string> > array, int line) {
    vector <string> result;
    result.push_back ("str");
    result.push_back (array [0] [0]);
    return result;
}

vector <string> data_len (vector < vector <string> > array, int line) {
    int len;
    if (array [0] [0] == "str") {
        len = array [0] [1].length ();
    } else if (array [0] [0] == "array") {
        vector <string> div = split (array [0] [1], " ");
        len = div.size ();
    } else {
        terminate ("data.len () does not take type <" + array [0] [0] + "> as a parameter", line);
    }
    vector <string> result;
    result.push_back ("num");
    result.push_back (to_string (len));
    return result;
}

void init_data () {
    string new_variables [2] = {"data.type", "data.len"};
    main_function new_functions [2] = {data_type, data_len};
    int size_holders = holders.size ();
    int size_functions = functions.size ();
    for (int index = 0; index < 2; index ++) {
        variables.push_back (new_variables [index]);
        pointers.push_back (size_holders + index);
        vector <string> value_set;
        value_set.push_back ("func");
        value_set.push_back (to_str (size_functions + index));
        holders.push_back (value_set);
        functions.push_back (new_functions [index]);
    }
}

vector <string> set (vector <string> command_list, string prefix, int start_line);

vector < vector <string> > excecute (string command, string prefix, int line) {
    vector <string> values;
    vector <string> frame;
    vector <int> degree;
    bool control = true;
    for (int index = 0; index < command.length (); index ++) {
        string unit;
        unit += command [index];
        if (unit != " ") {
            control = false;
        }
    }
    if (control) {
        vector < vector <string> > result_array;
        result_array.push_back (values);
        result_array.push_back (frame);
        return result_array;
    }
    string current = "";
    string info = "";
    int level = 0;
    int mode = 1;
    for (int index = 0; index < command.length (); index ++) {
        string unit;
        unit += command [index];
        int type = char_type (unit);
        if (unit == "`") {
            if (mode == - 1) {
                mode = 1;
            } else {
                if (mode != 1) {
                    values.push_back (current);
                    frame.push_back (info);
                    int new_degree = 0;
                    if (info == "sign") {
                        new_degree = level * (set_length + 1) + order (current) + 1;
                    }
                    degree.push_back (new_degree);
                }
                mode = - 1;
            }
        } else if (mode == - 1) {
            continue;
        } else if (type == 0) {
            if (mode != 0) {
                if (mode != 1) {
                    values.push_back (current);
                    frame.push_back (info);
                    int new_degree = 0;
                    if (info == "sign") {
                        new_degree = level * (set_length + 1) + order (current) + 1;
                    }
                    degree.push_back (new_degree);
                }
                current = "";
                info = "str";
                mode = 0;
            } else {
                values.push_back (current);
                frame.push_back ("str");
                degree.push_back (0);
                mode = 1;
            }
        } else if (mode == 0) {
            current += unit;
        } else if (unit == " ") {
            if (mode != 1) {
                values.push_back (current);
                frame.push_back (info);
                int new_degree = 0;
                if (info == "sign") {
                    new_degree = level * (set_length + 1) + order (current) + 1;
                }
                degree.push_back (new_degree);
            }
            current = "";
            info = "";
            mode = 1;
        } else if (type == 1 or type == 2) {
            if (mode != 1) {
                values.push_back (current);
                frame.push_back (info);
                int new_degree = 0;
                if (info == "sign") {
                    new_degree = level * (set_length + 1) + order (current) + 1;
                }
                degree.push_back (new_degree);
            }
            int new_degree = 0;
            if (type == 1) {
                level ++;
                new_degree = level * (set_length + 1) + 1;
            } else {
                level --;
            }
            values.push_back (unit);
            frame.push_back ("bracket");
            degree.push_back (new_degree);
            current = "";
            info = "";
            mode = 1;
        } else if (mode == 1) {
            if (type == 3) {
                current = unit;
                info = "sign";
                mode = 2;
            } else if (type == 4) {
                current = unit;
                info = "num";
                mode = 3;
            } else if (type == 5) {
                current = unit;
                info = "name";
                mode = 4;
            }
        } else if (mode == 2) {
            if (type == 3) {
                current += unit;
            } else {
                values.push_back (current);
                frame.push_back ("sign");
                degree.push_back (level * (set_length + 1) + order (current) + 1);
                if (type == 4) {
                    current = unit;
                    info = "num";
                    mode = 3;
                } else if (type == 5) {
                    current = unit;
                    info = "name";
                    mode = 4;
                }
            }
        } else if (mode == 3) {
            if (type == 4) {
                current += unit;
            } else {
                values.push_back (current);
                frame.push_back ("num");
                degree.push_back (0);
                if (type == 3) {
                    current = unit;
                    info = "sign";
                    mode = 2;
                } else if (type == 5) {
                    current = unit;
                    info = "name";
                    mode = 4;
                }
            }
        } else if (mode == 4) {
            if (type == 4 or type == 5) {
                current += unit;
            } else {
                values.push_back (current);
                frame.push_back ("name");
                degree.push_back (0);
                if (type == 3) {
                    current = unit;
                    info = "sign";
                    mode = 2;
                }
            }
        }
    }
    if (mode != 1) {
        values.push_back (current);
        frame.push_back (info);
        int new_degree = 0;
        if (info == "sign") {
            new_degree = level * (set_length + 1) + order (current) + 1;
        }
        degree.push_back (new_degree);
    }
    for (int index = 0; index < values.size (); index ++) {
        if (frame [index] == "name") {
            if (values [index] == "true" || values [index] == "false") {
                frame [index] = "bool";
            } else if (values [index] == "if" || values [index] == "elif" || values [index] == "else" || values [index] == "repeat" || values [index] == "loop" || values [index] == "main" || values [index] == "block" || values [index] == "return" || values [index] == "end") {
                terminate ("keyword '" + values [index] + "' cannot be used as a variable name", line);
            } else {
                int new_pointer;
                bool hit = false;
                for (int in = 0; in < variables.size (); in ++) {
                    if (prefix + values [index] == variables [in]) {
                        new_pointer = pointers [in];
                        hit = true;
                        break;
                    } else if (values [index] == variables [in]) {
                        new_pointer = pointers [in];
                        hit = true;
                    }
                }
                if (! hit) {
                    new_pointer = holders.size ();
                    variables.push_back (prefix + values [index]);
                    pointers.push_back (new_pointer);
                    vector <string> null;
                    null.push_back ("null");
                    null.push_back ("null");
                    holders.push_back (null);
                }
                values [index] = to_str (new_pointer);
                frame [index] = "object";
            }
        }
        if (frame [index] == "str" || frame [index] == "num" || frame [index] == "bool") {
            vector <string> value_set;
            value_set.push_back (frame [index]);
            value_set.push_back (values [index]);
            holders.push_back (value_set);
            values [index] = to_str (holders.size () - 1);
            frame [index] = "object";
        }
    }
    while (values.size () > 1) {
        int max = *max_element (degree.begin (), degree.end ());
        if (max == 0) {
            break;
        }
        int axis = 0;
        while (degree [axis] != max) {
            axis ++;
        }
        string sign = values [axis];
        string answer;
        string new_type;
        int left;
        int right;
        bool actions [2] = {false, false};
        if (sign == "(") {
            if (frame [axis - 1] == "object") {
                int base_pointer = to_int (values [axis - 1]);
                vector <string> func_obj = holders [base_pointer];
                if (func_obj [0] == "func") {
                    int func_pointer = to_int (func_obj [1]);
                    main_function func = functions [func_pointer];
                    vector < vector <string> > params;
                    vector <int> save;
                    int array_index = axis + 1;
                    while (values [array_index] != ")") {
                        if (values [array_index] == "$") {
                            if (frame [array_index + 1] != "object") {
                                terminate ("improper usage of '$' operator", line);
                            }
                            int index = to_int (values [array_index + 1]);
                            save.push_back (index);
                        }
                        else if (values [array_index] != "|") {
                            int index = to_int (values [array_index]);
                            vector <string> value_set = holders [index];
                            params.push_back (value_set);
                        }
                        array_index ++;
                    }
                    vector <string> result = func (params, line);
                    for (int index = 0; index < save.size (); index ++) {
                        holders [save [index]] = result;
                    }
                    answer = result [1];
                    new_type = result [0];
                    left = - 1;
                    right = array_index - axis;
                    actions [0] = true;
                    actions [1] = true;
                } else if (func_obj [0] == "block") {
                    vector <string> indices = split (func_obj [1], " ");
                    vector < vector <string> > initial = excecute (func_obj [2], to_string (base_pointer) + "$", to_int (indices [0]) - 1);
                    vector < vector <string> > declaration;
                    vector <string> empty;
                    declaration.push_back (empty);
                    declaration.push_back (empty);
                    for (int index = 0; index < initial [0].size (); index ++) {
                        if (initial [0] [index] != "|") {
                            declaration [0].push_back (initial [0] [index]);
                            declaration [1].push_back (initial [1] [index]);
                        }
                    }
                    vector < vector <string> > params;
                    vector <int> save;
                    int array_index = axis + 1;
                    while (values [array_index] != ")") {
                        if (values [array_index] == "$") {
                            if (frame [array_index + 1] != "object") {
                                terminate ("improper usage of '$' operator", line);
                            }
                            int index = to_int (values [array_index + 1]);
                            save.push_back (index);
                        }
                        else if (values [array_index] != "|") {
                            int index = to_int (values [array_index]);
                            vector <string> value_set = holders [index];
                            params.push_back (value_set);
                        }
                        array_index ++;
                    }
                    for (int index = 0; index < declaration [0].size (); index ++) {
                        int pointer = to_int (declaration [0] [index]);
                        vector <string> value_set;
                        holders [pointer] = params [index];
                    }
                    vector <string> mega_list;
                    for (int index = to_int (indices [0]); index < to_int (indices [1]); index ++) {
                        mega_list.push_back (master_list [index]);
                    }
                    vector <string> value_set = set (mega_list, to_string (base_pointer) + "$", to_int (indices [0]));
                    for (int index = 0; index < save.size (); index ++) {
                        holders [save [index]] = value_set;
                    }
                    answer = value_set [1];
                    new_type = value_set [0];
                    left = - 1;
                    right = array_index - axis;
                    actions [0] = true;
                    actions [1] = true;
                } else {
                    terminate ("type <" + func_obj [0] + "> is not callable", line);
                }
            } else {
                if (values [axis + 2] != ")") {
                    terminate ("multiple values found inside parantheses", line);
                }
                answer = values [axis + 1];
                left = 0;
                right = 2;
                actions [1] = true;
            }
        } else if (sign == "[") {
            if (frame [axis - 1] == "object") {
                int pointer_1 = to_int (values [axis - 1]);
                int pointer_2 = to_int (values [axis + 1]);
                vector <string> value_obj_1 = holders [pointer_1];
                vector <string> value_obj_2 = holders [pointer_2];
                if (value_obj_1 [0] != "array") {
                    terminate ("type <" + value_obj_1 [0] + "> cannot be sliced", line);
                }
                if (value_obj_2 [0] != "num") {
                    terminate ("type <" + value_obj_2 [0] + "> cannot be provided as an index for slicing", line);
                }
                string value_1 = holders [pointer_1] [1];
                int value_2 = to_num (holders [pointer_2] [1]);
                vector <string> indices = split (value_obj_1 [1], " ");
                answer = indices [to_int (value_obj_2 [1])];
                left = - 1;
                right = 2;
                actions [1] = true;
            } else {
                string indices;
                int array_index = axis + 1;
                while (values [array_index] != "]") {
                    if (values [array_index] != "|") {
                        indices += " " + to_string (holders.size ());
                        holders.push_back (holders [to_int (values [array_index])]);
                    }
                    array_index ++;
                }
                if (array_index != axis + 1) {
                    indices = indices.substr (1, indices.length () - 1);
                }
                answer = indices;
                new_type = "array";
                left = 0;
                right = array_index - axis;
                actions [0] = true;
                actions [1] = true;
            }
        } else if (sign == "+") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) + (double) to_num (values_2 [1]));
                new_type = "num";
            } else if (values_1 [0] == "str" && values_2 [0] == "str") {
                answer = values_1 [1] + values_2 [1];
                new_type = "str";
            } else if (values_1 [0] == "array" && values_2 [0] == "array") {
                if (values_1 [1] == "" && values_2 [1] == "") {
                    answer = "";
                } else if (values_1 [1] == "") {
                    answer = values_2 [1];
                } else if (values_2 [1] == "") {
                    answer = values_1 [1];
                } else {
                    answer = values_1 [1] + " " + values_2 [1];
                }
                new_type = "array";
            } else {
                terminate ("the '+' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "+=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) + (double) to_num (values_2 [1]));
                new_type = "num";
            } else if (values_1 [0] == "str" && values_2 [0] == "str") {
                answer = values_1 [1] + values_2 [1];
                new_type = "str";
            } else if (values_1 [0] == "array" && values_2 [0] == "array") {
                if (values_1 [1] == "" && values_2 [1] == "") {
                    answer = "";
                } else if (values_1 [1] == "") {
                    answer = values_2 [1];
                } else if (values_2 [1] == "") {
                    answer = values_1 [1];
                } else {
                    answer = values_1 [1] + " " + values_2 [1];
                }
                new_type = "array";
            } else {
                terminate ("the '+=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "-") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) - (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '-' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "-=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) - (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '-=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "*") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) * (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '*' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "*=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) * (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '*=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "/") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) / (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '/' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "/=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) to_num (values_1 [1]) / (double) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '/=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "^") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) pow ((double) to_num (values_1 [1]), (double) to_num (values_2 [1])));
                new_type = "num";
            } else {
                terminate ("the '^' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "^=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((double) pow ((double) to_num (values_1 [1]), (double) to_num (values_2 [1])));
                new_type = "num";
            } else {
                terminate ("the '^=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "#") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((int) to_num (values_1 [1]) % (int) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '#' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "#") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                answer = to_str ((int) to_num (values_1 [1]) % (int) to_num (values_2 [1]));
                new_type = "num";
            } else {
                terminate ("the '#' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders [pointer_1] = value_set;
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "<") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) < (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '<' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == ">") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) > (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '>' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "==") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) == (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else if (values_1 [0] == "str" && values_2 [0] == "str") {
                bool exp = values_1 [1] == values_2 [1];
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else if (values_1 [0] == "bool" && values_2 [0] == "bool") {
                bool exp = values_1 [1] == values_2 [1];
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '==' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "<=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) <= (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '<=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == ">=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) >= (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '>=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "!=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "num" && values_2 [0] == "num") {
                bool exp = (double) to_num (values_1 [1]) != (double) to_num (values_2 [1]);
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else if (values_1 [0] == "str" && values_2 [0] != "str") {
                bool exp = values_1 [1] == values_2 [1];
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else if (values_1 [0] == "bool" && values_2 [0] != "bool") {
                bool exp = values_1 [1] == values_2 [1];
                if (exp) {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '!=' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "%") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "bool" && values_2 [0] == "bool") {
                if (values_1 [1] == "false" & values_2 [1] == "false") {
                    answer = "false";
                } else {
                    answer = "true";
                }
                new_type = "bool";
            } else {
                terminate ("the '%' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "&") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> values_1 = holders [pointer_1];
            vector <string> values_2 = holders [pointer_2];
            if (values_1 [0] == "bool" && values_2 [0] == "bool") {
                if (values_1 [1] == "true" & values_2 [1] == "true") {
                    answer = "true";
                } else {
                    answer = "false";
                }
                new_type = "bool";
            } else {
                terminate ("the '&' operator is not supported between type <" + values_1 [0] + "> and type <" + values_2 [0] + ">", line);
            }
            left = - 1;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "!") {
            int pointer = to_int (values [axis + 1]);
            vector <string> values = holders [pointer];
            if (values [0] == "bool") {
                if (values [1] == "true") {
                    answer = "false";
                } else {
                    answer = "true";
                }
                new_type = "bool";
            } else {
                terminate ("the '&' operator is not supported for type <" + values [0] + ">", line);
            }
            left = 0;
            right = 1;
            actions [0] = true;
            actions [1] = true;
        } else if (sign == "=") {
            int pointer_1 = to_int (values [axis - 1]);
            int pointer_2 = to_int (values [axis + 1]);
            vector <string> value_set = holders [pointer_2];
            holders [pointer_1] = value_set;
            answer = to_str (pointer_1);
            left = - 1;
            right = 1;
            actions [1] = true;
        } else {
            degree [axis] = 0;
        }
        if (actions [0]) {
            vector <string> value_set;
            value_set.push_back (new_type);
            value_set.push_back (answer);
            holders.push_back (value_set);
            answer = to_str (holders.size () - 1);
        }
        if (actions [1]) {
            values.erase (values.begin () + axis + left, values.begin () + axis + right + 1);
            frame.erase (frame.begin () + axis + left, frame.begin () + axis + right + 1);
            degree.erase (degree.begin () + axis + left, degree.begin () + axis + right + 1);
            values.insert (values.begin () + axis + left, answer);
            frame.insert (frame.begin () + axis + left, "object");
            degree.insert (degree.begin () + axis + left, 0);
        }
    }
    vector < vector <string> > result_array;
    result_array.push_back (values);
    result_array.push_back (frame);
    return result_array;
}

vector <string> set (vector <string> command_list, string prefix, int start_line) {
    int sub_line = 0;
    while (sub_line < command_list.size ()) {
        string command = command_list [sub_line];
        vector <string> result = index_unit (command);
        if (result [0] == "if") {
            vector < vector < vector <string> > > mega_list;
            vector <string> condition;
            condition.push_back (result [1]);
            vector <string> sub_list;
            int push_line = start_line + sub_line + 1;
            int level = 0;
            while (true) {
                sub_line ++;
                vector <string> result = index_unit (command_list [sub_line]);
                if (result [0] == "elif" && level == 0) {
                    vector < vector <string> > block;
                    block.push_back (condition);
                    block.push_back (sub_list);
                    mega_list.push_back (block);
                    sub_list.clear ();
                    condition.clear ();
                    condition.push_back (result [1]);
                } else if (result [0] == "else" && level == 0) {
                    vector < vector <string> > block;
                    block.push_back (condition);
                    block.push_back (sub_list);
                    mega_list.push_back (block);
                    sub_list.clear ();
                    condition.clear ();
                    condition.push_back ("true");
                } else if (result [0] == "end" && level == 0) {
                    vector < vector <string> > block;
                    block.push_back (condition);
                    block.push_back (sub_list);
                    mega_list.push_back (block);
                    break;
                } else {
                    if (result [0] == "if" || result [0] == "repeat" || result [0] == "loop") {
                        level ++;
                    } else if (result [0] == "end") {
                        level --;
                    }
                    sub_list.push_back (command_list [sub_line]);
                }
            }
            for (int index = 0; index < mega_list.size (); index ++) {
                vector < vector <string> > output;
                output = excecute (mega_list [index] [0] [0], prefix, push_line - 1);
                int pointer = to_int (output [0] [0]);
                vector <string> value_set = holders [pointer];
                if (value_set [0] == "bool" & value_set [1] == "true") {
                    set (mega_list [index] [1], prefix, push_line);
                    break;
                }
            }
            sub_line ++;
        } else if (result [0] == "repeat") {
            vector <string> mega_list;
            string condition = result [1];
            int push_line = start_line + sub_line + 1;
            int level = 0;
            while (true) {
                sub_line ++;
                vector <string> result = index_unit (command_list [sub_line]);
                if (result [0] == "end" && level == 0) {
                    break;
                } else {
                    if (result [0] == "if" || result [0] == "repeat" || result [0] == "loop") {
                        level ++;
                    } else if (result [0] == "end") {
                        level --;
                    }
                    mega_list.push_back (command_list [sub_line]);
                }
            }
            while (true) {
                vector < vector <string> > output;
                output = excecute (condition, prefix, push_line - 1);
                int pointer = to_int (output [0] [0]);
                vector <string> value_set = holders [pointer];
                if (value_set [0] == "bool" & value_set [1] == "true") {
                    set (mega_list, prefix, push_line);
                } else {
                    break;
                }
            }
            sub_line ++;
        } else if (result [0] == "loop") {
            vector <string> mega_list;
            string condition = result [1];
            int push_line = start_line + sub_line + 1;
            int level = 0;
            while (true) {
                sub_line ++;
                vector <string> result = index_unit (command_list [sub_line]);
                if (result [0] == "end" && level == 0) {
                    break;
                } else {
                    if (result [0] == "if" || result [0] == "repeat" || result [0] == "loop") {
                        level ++;
                    } else if (result [0] == "end") {
                        level --;
                    }
                    mega_list.push_back (command_list [sub_line]);
                }
            }
            vector < vector <string> > output;
            output = excecute (result [1], prefix, push_line - 1);
            vector < vector <string> > relevant;
            vector <string> empty;
            relevant.push_back (empty);
            relevant.push_back (empty);
            for (int index = 0; index < output [0].size (); index ++) {
                if (output [0] [index] != "|") {
                    relevant [0].push_back (output [0] [index]);
                    relevant [1].push_back (output [1] [index]);
                }
            }
            int pointer = to_int (relevant [0] [2]);
            vector <string> value_set = holders [pointer];
            vector <string> pointer_array = split (value_set [1], " ");
            for (int index = 0; index < pointer_array.size (); index ++) {
                int pointer_relevant = to_int (pointer_array [index]);
                vector <string> value_set_relevant = holders [pointer_relevant];
                int pointer_1 = to_int (relevant [0] [0]);
                holders [pointer_1] [0] = "num";
                holders [pointer_1] [1] = to_string (index);
                int pointer_2 = to_int (relevant [0] [1]);
                holders [pointer_2] = value_set_relevant;
                set (mega_list, prefix, push_line);
            }
            sub_line ++;
        } else if (result [0] == "return") {
            vector < vector <string> > output = excecute (result [1], prefix, sub_line);
            int pointer = to_int (output [0] [0]);
            return holders [pointer];
        } else {
            excecute (command, prefix, start_line + sub_line);
            sub_line ++;
        }
    }
    vector <string> value_set;
    value_set.push_back ("null");
    value_set.push_back ("null");
    return value_set;
}

int main (int places, char *args []) {
    if (places == 1) {
        cout << "MAGNUS - no arguements given" << endl;
    } else {
        if (places == 2) {
            init_cui ();
            init_prs ();
            init_data ();
            fstream program;
            program.open (args [1], ios::in);
            string command;
            vector <string> command_list;
            while (getline (program, command)) {
                command_list.push_back (command);
            }
            program.close ();
            master_list = command_list;
            vector <string> main_list;
            int main_line;
            int sub_line = 0;
            while (sub_line < command_list.size ()) {
                vector <string> result = index_unit (command_list [sub_line]);
                if (result [0] == "main") {
                    main_line = sub_line + 1;
                    int level = 0;
                    while (true) {
                        sub_line ++;
                        vector <string> result = index_unit (command_list [sub_line]);
                        if (result [0] == "if" || result [0] == "repeat" || result [0] == "loop") {
                            level ++;
                        } else if (result [0] == "end") {
                            if (level == 0) {
                                break;
                            }
                            level --;
                        }
                        main_list.push_back (command_list [sub_line]);
                    }
                } else if (result [0] == "block") {
                    vector <string> sub_result;
                    sub_result = index_unit (result [1]);
                    string name = sub_result [0];
                    int start_line = sub_line + 1;
                    int level = 0;
                    bool left_bracket = true;
                    string inter = "";
                    for (int index = 0; index < sub_result [1].length (); index ++) {
                        string unit;
                        unit += sub_result [1] [index];
                        if (unit == "(" && left_bracket) {
                            left_bracket = false;
                        } else {
                            inter += unit;
                        }
                    }
                    bool right_bracket = true;
                    string declaration = "";
                    for (int index = inter.length () - 1; index >= 0; index --) {
                        string unit;
                        unit += inter [index];
                        if (unit == ")" && right_bracket) {
                            right_bracket = false;
                        } else {
                            declaration = unit + declaration;
                        }
                    }
                    if (left_bracket || right_bracket) {
                        terminate ("invalid syntax for block declaration", start_line - 1);
                    }
                    while (true) {
                        sub_line ++;
                        vector <string> result = index_unit (command_list [sub_line]);
                        if (result [0] == "if" || result [0] == "repeat" || result [0] == "loop") {
                            level ++;
                        } else if (result [0] == "end") {
                            if (level == 0) {
                                break;
                            }
                            level --;
                        }
                    }
                    int end_line = sub_line;
                    variables.push_back (name);
                    pointers.push_back (holders.size ());
                    vector <string> value_set;
                    value_set.push_back ("block");
                    value_set.push_back (to_string (start_line) + " " + to_string (end_line));
                    value_set.push_back (declaration);
                    holders.push_back (value_set);
                }
                sub_line ++;
            }
            set (main_list, "", main_line);
        } else {
            cout << "MAGNUS - too many arguements provided" << endl;
        }
    }
}
