// Basic Algebra Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <random>
#include <string>

//Generates a random double between a and b
double randdouble(double a, double b) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(a, b);
	return(dis(gen));
}

//Generates a random int between a and b
int randint(int a, int b) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(a, b);
	return(dis(gen));
}

//Generates a random int between a and b and then has a 50% chance of making it negative
int randint_wn(int a, int b) {
	int hold = randint(a, b);
		if (randdouble(0.0, 1.0) >= 0.5) {
			return(hold);
		}
		else {
			return(-1*hold);
		}
}

//prints the values of the equation array
void print_equ(const std::array<int, 6>& equ_vals) {
	for (int i = 0; i < 6; ++i) {
		std::cout << equ_vals[i] << std::endl;
	}
}

//Generates the numbers used to build the equation for type 1
std::array<int, 6> gen_vals_1(const std::array<double, 4> &const_probs, const std::array<double, 4> &linear_probs) {
	std::array<int, 6> hold;
	if (randdouble(0.0, 1.0) < linear_probs[1]) { //set a = 1 or to a random number
		hold[0] = 1;
	} else {
		hold[0] = randint_wn(1, 5);
	}
	if (randdouble(0.0, 1.0) < linear_probs[2]) { //set c = 1,0, or to a random number that doesn't cancel with a
		hold[1] = 0;
	} else if (randdouble(0.0, 1.0) < linear_probs[3] && hold[0] != 1) {
		hold[1] = 1;
	} else {
		bool T = true;
		while (T == true) {
			int test = randint(-5, 5);
			if (test != hold[0]) {
				hold[1] = test;
				T = false;
			}
		}
	}
	for (int i = 0; i < 4; ++i) { //sets up the constants so that equation gives int answer
		if (randdouble(0.0, 1.0) < const_probs[i]) {
			hold[i + 2] = (hold[0] - hold[1]) * randint_wn(1, 10);
		}
		else {
			hold[i + 2] = 0;
		}
	}
	if (hold[0] == 1 && hold[2] == 0) {
		hold[2] = (hold[0] - hold[1]) * randint_wn(1, 10);
	}
	return hold;
}

//Generates the numbers used to build the equation for type 2
std::array<int, 6> gen_vals_2(const std::array<double, 4>& const_probs, const std::array<double, 4>& linear_probs) {
    std::array<int, 6> hold;
    //set c first
    if (randdouble(0.0, 1.0) < linear_probs[2]) {//set c = 0
        hold[1] = 0;
    }
    else if (randdouble(0.0, 1.0) < linear_probs[3]) {//set c = 1
        hold[1] = 1;
    }
    else {
        hold[1] = randint_wn(2, 6);
    }
    //set f to a number or 0
    if (randdouble(0.0, 1.0) < const_probs[3]) {
        hold[5] = randint_wn(1, 10);
    }
    else {
        hold[5] = 0;
    }
    //set e to a number or 0
    if (randdouble(0.0, 1.0) < const_probs[2]) {
        hold[4] = randint_wn(1, 10);
    }
    else {
        hold[4] = 0;
    }
    //For when f-e=0 making the equation reduce to ax+b=0
    if (hold[5] - hold[4] == 0) {
        //set a = 1 or to a random number
        if (randdouble(0.0, 1.0) < linear_probs[1]) {
            hold[0] = 1;
        }
        else {
            hold[0] = randint_wn(2, 6);
        }
        //set b to a random number or to 0
        if (randdouble(0.0, 1.0) < const_probs[0] || hold[0] == 0) {
            hold[2] = hold[0] * randint_wn(1, 10);
        }
        else {
            hold[2] = 0;
        }
        //set d to a random number or to 0
        if (randdouble(0.0, 1.0) < const_probs[1] || hold[1] == 0) {
            hold[3] = randint_wn(1, 10);
        }
        else {
            hold[3] = 0;
        }
    }
    //For when f-e != 0
    else {
        //for when c = 0
        //set a
        if (hold[1] == 0) {
            if (randdouble(0.0, 1.0) < linear_probs[1]) {
                hold[0] = 1;
            }
            else {
                hold[0] = randint_wn(1, 5);
            }
            //set b
            if (randdouble(0.0, 1.0) < const_probs[0] || hold[0] == 0) {
                hold[2] = hold[0] * randint_wn(1, 10);
            }
            else {
                hold[2] = 0;
            }
            //set d
            hold[2] = hold[0] * randint_wn(1, 10);
            }
        //for c != 0
        else{
            //set a
            if (randdouble(0.0, 1.0) < linear_probs[0]) {
                hold[0] = 0;
            }
            else if (randdouble(0.0, 1.0) < linear_probs[1]) {
                hold[0] = 1;
            }
            else { //set a = random value that doesn't delete the denominator
                bool T = true;
                while (T == 1) {
                    int test = randint(-5, 5);
                    if (test - (hold[5] - hold[4]) * hold[1] != 0) {
                        hold[0] = test;
                        T = false;
                    }
                }
            }
            //set b
            if (randdouble(0.0, 1.0) < const_probs[0] || hold[0] == 0) {
                hold[2] = (hold[0] - (hold[5] - hold[4]) * hold[1]) * randint_wn(1, 10);
            }
            else {
                hold[2] = 0;
            }
            //set d
            if (randdouble(0.0, 1.0) < const_probs[1] || hold[1] == 0) {
                hold[3] = (hold[0] - (hold[5] - hold[4]) * hold[1]) * randint_wn(1, 10);
            }
            else {
                hold[3] = 0;
            }
        }
    }
    return(hold);
}

//Takes the equation array and returns the solution to the equation
int get_equ_ans(const std::array<int, 6>& equ_vals, int equ_type) {
	if (equ_type == 1) {
		return((equ_vals[3] - equ_vals[2]) / (equ_vals[0] - equ_vals[1]));
	}
	if (equ_type == 2) {
		return(((equ_vals[5] - equ_vals[4])*equ_vals[3] - equ_vals[2])/(equ_vals[0] - (equ_vals[3] - equ_vals[2])*equ_vals[1]));
	}
}

//Takes the equation array and returns a string to display the equation in console.
std::string display_equ(const std::array<int, 6>& equ, int equ_type) {
    if (equ_type == 1 || equ_type == 3) {
        std::string hold = "";
        if (equ[0] == 1) {
            hold += "x";
        }
        else if (equ[0] == -1) {
            hold += "-x";
        }
        else if (equ[0] != 0) {
            hold += std::to_string(equ[0]) + "x";
        }
        if (equ[2] == 0) {
            hold += " = ";
        }
        else if (equ[2] > 0 && equ[0] != 0) {
            hold += " + " + std::to_string(equ[2]) + " = ";
        }
        else if (equ[2] < 0 && equ[0] != 0) {
            hold += " - " + std::to_string(-equ[2]) + " = ";
        }
        else if (equ[2] > 0 && equ[0] == 0) {
            hold += std::to_string(equ[2]) + " = ";
        }
        else if (equ[2] < 0 && equ[0] == 0) {
            hold += "-" + std::to_string(-equ[2]) + " = ";
        }
        if (equ_type == 3) {
            hold += std::to_string(equ[5]) + "(";
        }
        if (equ[1] == 1) {
            hold += "x";
                }
        else if (equ[1] == -1) {
            hold += "-x";
        }
        else if (equ[1] != 1 && equ[1] != 0) {
            hold += std::to_string(equ[1]) + "x";
        }
        if (equ[1] != 0 && equ[3] > 0) {
            hold += " + " + std::to_string(equ[3]);
        }
        else if (equ[1] != 0 && equ[3] < 0) {
            hold += " - " + std::to_string(-equ[3]);
        }
        else if (equ[3] > 0 && equ[1] == 0) {
            hold += std::to_string(equ[3]);
        }
        else if (equ[3] < 0 && equ[1] == 0) {
            hold += "-" + std::to_string(-equ[3]);
        }
        else if (equ[3] == 0 && equ[1] == 0) {
            hold += "0";
        }
        if (equ_type == 3) {
            hold += ")";
        }
        return(hold);
    }
    if (equ_type == 2) {
        std::string num = "";
        if (equ[0] == 1) {
            num += "x";
        }
        else if (equ[0] == -1) {
            num += "-x";
        }
        else if (equ[0] != 1 && equ[0] != 0) {
            num += std::to_string(equ[0]) + "x";
        }
        if (equ[0] != 0 && equ[2] > 0) {
            num += " + " + std::to_string(equ[2]);
        }
        else if (equ[0] != 0 && equ[2] < 0) {
            num += " - " + std::to_string(-equ[2]);
        }
        else if (equ[2] > 0 && equ[0] == 0) {
            num += std::to_string(equ[2]);
        }
        else if (equ[2] < 0 && equ[0] == 0) {
            num += "-" + std::to_string(-equ[2]);
        }
        else if (equ[2] == 0 && equ[0] == 0) {
            num += "0";
        }
        std::string den = "";
        if (equ[1] == 1) {
            den += "x";
            }
        else if (equ[1] == -1) {
            den += "-x";
        }
        else if (equ[1] != 1 && equ[1] != 0) {
            den += std::to_string(equ[1]) + "x";
        }
        if (equ[3] != 0 && equ[1] != 0 && equ[3] > 0) {
            den += " + " + std::to_string(equ[3]);
        }
        else if (equ[3] != 0 && equ[1] != 0 && equ[3] < 0) {
            den += " - " + std::to_string(-equ[3]);
        }
        else if (equ[3] > 0 && equ[1] == 0) {
            den += std::to_string(equ[3]);
        }
        else if (equ[3] < 0 && equ[1] == 0) {
            den += "-" + std::to_string(-equ[3]);
        }
        else if (equ[3] == 0 && equ[1] == 0) {
            den += "0";
        }
        int fraclen = 0;
        if (num.length() >= den.length()) {
            fraclen = num.length() + 2;
        }
        else{
            fraclen = den.length() + 2;
        }
        int nummar = int((fraclen - num.length())/2);
        int denmar = int((fraclen - den.length())/2);
        std::string hold = "";
        hold += std::string(nummar, ' ') + num + std::string(nummar, ' ') + "\n";
        //old unicode for fraction bar:─\u2500
        hold += std::string(fraclen, '-');
        if (equ[4] > 0) {
            hold += " + " + std::to_string(equ[4]);
        }
        else if (equ[4] < 0) {
            hold += " - " + std::to_string(-equ[4]);
        }
        hold += " = " + std::to_string(equ[5]) + "\n";
        hold += std::string(denmar, ' ') + den + std::string(denmar, ' ');
        return(hold);
    }
}

//room for arr-int if necessary

//Takes the array of a type 1 equation and returns a string displaying each step of the solution
std::string show_sol_1(const std::array<int, 6>& equ) {
    std::array<int, 6> temp_equ = equ;
    std::string hold = "";
    hold += display_equ(temp_equ, 1);
    if (temp_equ[2] != 0) {
        hold += "Take " + std::to_string(temp_equ[2]) + " to the right hand side.\n";
        temp_equ[3] -= temp_equ[2];
        temp_equ[2] = 0;
        hold += display_equ(temp_equ, 1) + "/n/n";
    }
    if (temp_equ[1] != 0) {
        hold += "Take " + std::to_string(temp_equ[1]) + "x to the left hand side.\n";
        temp_equ[0] -= temp_equ[1];
        temp_equ[1] = 0;
        hold += display_equ(temp_equ, 1) + "/n/n";
    }
    if (temp_equ[0] != 1) {
        hold += "Divide by " + std::to_string(temp_equ[0]) + ".\n";
        temp_equ[3] /= temp_equ[0];
        temp_equ[0] = 1;
        hold += display_equ(temp_equ, 1) + "/n/n";
    }
    hold += "Done!";
    return(hold);
}

//Takes the array of a type 2 equation and returns a string displaying each step of the solution
std::string show_sol_2(const std::array<int, 6>& equ) {
    std::array<int, 6> temp_equ = equ;
    std::string hold = "";
    hold += display_equ(temp_equ, 2);
    if (temp_equ[4] != 0) {
        hold += "Take " + std::to_string(temp_equ[4]) + " to the right hand side.\n";
        temp_equ[5] -= temp_equ[4];
        temp_equ[4] = 0;
        hold += display_equ(temp_equ, 2) + "/n/n";
    }
    hold += "Multiply both sides by the denominator.\n";
    hold += display_equ(temp_equ, 3) + "/n/n";
    hold += "Multiply out the brackets. \n";
    temp_equ[1] *= temp_equ[5];
    temp_equ[3] *= temp_equ[5];
    if (temp_equ[0] == 0) {
        hold += display_equ(temp_equ, 1) + "/n/n";
        hold += "Switch the sides. \n";
        int b = temp_equ[2];
        int c = temp_equ[1];
        int d = temp_equ[3];
        temp_equ[0] = c;
        temp_equ[1] = 0;
        temp_equ[2] = d;
        temp_equ[3] = b;
    }
    hold += show_sol_1(temp_equ);
    return(hold);
}

//Generates an equation and offers answer attempt or solution
int algebra_loop(double type_prob, const std::array<double, 4>& const_probs, const std::array<double, 4>& linear_probs) {
    std::cout << std::string(80, '\u2550') << std::endl;
    while (true) {
        std::cout << "Please type 'n' for a random equation, or 'l' to return to level select." << std::endl;
        std::string x;
        std::cin >> x;
        if (x == 'n' || x == 'N') {
            break;
        }
        else if (x == 'l' || x == 'L') {
            return(0);
        }
        else {
            std::cout << "Command unknown." << std::endl;
        }
    }
    int equ_type;
    std::array<int, 6> equ;
    if (randdouble(0.0, 1.0) < type_prob) {
        equ_type = 1;
        equ = gen_vals_1(const_probs, linear_probs);
    }
    else {
        equ_type = 2;
        equ = gen_vals_2(const_probs, linear_probs);
    }
    std::string ans = std::to_string(get_equ_ans(equ, equ_type));
    std::string sol;
    if (equ_type == 1) {
        sol = show_sol_1(equ);
    }
    else {
        sol = show_sol_2(equ);
    }
    std::cout << std::string(80, '\u2550') << std::endl;
    std::cout << display_equ(equ, equ_type) << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "Type your answer or type 's' to see the solution" << std::endl;
    while (true) {
        std::string y;
        std::cin >> y;
        if (y == ans) {
            std::cout << "Correct, well done!" << std::endl;
            break;
        }
        else if (y != ans && y != 's' && y != 'S' && y != 'd' && y != 'D') {
            std::cout << "Sorry! Try again or type 's' to see the solution." << std::endl;
        }
        else if (y == 's' || y == 'S') {
            std::cout << std::string(80, '\u2550') << std::endl;
            std::cout << sol << std::endl;
            break;
        }
        else if (y == 'd' || y == 'D') {
            print_equ(equ);
            std::cout << ans << std::endl;
        }
        else {
            std::cout << "Unknown command. Please try again." << std::endl;
            tefaa
        }
    }
    return(1);
}

int main()
{
	//probabilities
	double type_prob = 1;
	std::array<double, 4> const_probs { 0.5, 0.5, 0.5, 0.5 };
	std::array<double, 4> linear_probs { 0.5, 0.5, 0.5, 0.5};

    std::array<double, 5> type_probs_by_level{ 1, 1, 0.5, 0.3, 0 };
    std::array<std::array<int, 4>, 5> const_probs_by_level{ { 1, 1, 0.5, 0.3 }, { 1, 1, 0.5, 0.3 }, { 1, 1, 0.5, 0.3 }, { 1, 1, 0.5, 0.3 }, { 1, 1, 0.5, 0.3 } };


	//manually selected variables for testing
	int equ_type = 2;

	std::array<int, 6> test = gen_vals_2(const_probs, linear_probs);
	print_equ(test);
    std::cout << display_equ(test, 2) << std::endl;
}