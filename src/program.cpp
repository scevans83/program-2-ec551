#include "../include/program.h"

int main()
{
    BooleanExpression expression;
    map<string, pair<string, vector<string>>> circuit;

    cout << "What would you like to do?\n";
    cout << "1. Input a Boolean algebraic function\n";
    cout << "2. Input a digital combinational logic circuit\n";
    cout << "\nEnter choice (1 or 2): ";

    int choice;
    cin >> choice;
    cin.ignore();

    string inputData;
    if (choice == 1)
    {
        cout << "\nEnter the Boolean algebraic function: ";
        getline(cin, inputData);
        expression = parseBoolean(inputData);
    }
    else if (choice == 2)
    {
        cout << "\nEnter the digital combinational logic circuit following the below example: \n";
        cout << "(for more information, see https://course.ece.cmu.edu/~ee760/760docs/blif.pdf)\n";
        cout << "\n.model simple_model\n";
        cout << ".inputs a b c\n";
        cout << ".outputs f\n";
        cout << ".names a b c f\n";
        cout << "001 1\n";
        cout << "011 1\n";
        cout << "100 1\n";
        cout << "\n.end\n";
        cout << "\nyour input:";
        cout << endl;
        expression = getInputCircuit();
    }
    else
    {
        cerr << "Invalid choice. Please run the program again." << endl;
        return 1;
    }

    /*prompt the user to select what they want to do from the following list:
        1. Return the design as a canonical SOP
        2. Return the design as a canonical POS
        3. Return the design INVERSE as a canonical SOP
        4. Return the design INVERSE as a canonical POS
        5. Return a minimized number of literals representation in SOP
            a. Report on the number of saved literals vs. the canonical version
        6. Return a minimized number of literals representation in POS
            a. Report on the number of saved literals vs. the canonical version
        7. Report the number of Prime Implicants
        8. Report the number of Essential Prime Implicants
        9. Report the number of ON-Set minterms
        10. Report the number of ON-Set maxterms
    */
    cout << "\nWhat would you like to do with this data?\n";
    cout << " 1. Return the design as a canonical SOP\n";
    cout << " 2. Return the design as a canonical POS\n";
    cout << " 3. Return the design INVERSE as a canonical SOP\n";
    cout << " 4. Return the design INVERSE as a canonical POS\n";
    cout << " 5. Return a minimized number of literals representation in SOP\n";
    cout << " 6. Return a minimized number of literals representation in POS\n";
    cout << " 7. Report the number of Prime Implicants\n";
    cout << " 8. Report the number of Essential Prime Implicants\n";
    cout << " 9. Report the number of ON-Set minterms\n";
    cout << "10. Report the number of ON-Set maxterms\n";
    cout << "11. Draw SOP circuit as ASCII\n";
    cout << "12. Draw POS circuit as ASCII\n";
    cout << "\nEnter choice (1-12): ";

    int choice2;
    cin >> choice2;
    cin.ignore();
    vector<string> PI = primeImplicants(expression);
    cout << endl;
    switch (choice2)
    {
    case 1:
        canonicalSOP(expression);
        break;
    case 2:
        canonicalPOS(expression);
        break;
    case 3:
        inverseCanonicalSOP(expression);
        break;
    case 4:
        inverseCanonicalPOS(expression);
        break;
    case 5:
        minimizedSOP(expression);
        break;
    case 6:
        minimizedPOS(expression);
        break;
    case 7:
        for (const string &prime : PI)
        {
            cout << "Prime Implicant: " << prime << endl;
        }
        cout << "Number of Prime Implicants: " << PI.size() << endl;
        break;
    case 8:
        essentialPrimeImplicants(expression);
        break;
    case 9:
        onSetMinterms(expression);
        break;
    case 10:
        onSetMaxterms(expression);
        break;
    case 11:
        asciiSOP(expression);
        break;
    case 12:
        asciiPOS(expression);
        break;
    default:
        cerr << "Invalid choice. Please try again." << endl;
        return 1;
    }

    return 0;
}