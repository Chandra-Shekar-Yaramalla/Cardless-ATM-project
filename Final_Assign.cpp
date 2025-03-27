#include<bits/stdc++.h>
using namespace std;
fstream operations("ATM_data.txt",ios::in | ios::out | ios::ate);   // File Initialization.

// Functions Declarations.
int store(string p);
void debit();
void check_balance();
void credit();
void modify_pin();
int is_all_num(string);
int num_of_digits(long long int);
void clear_bal(streampos,int);

// Global Variables.
string phone,phone_t;
string pin;
long long int balance;

class service
{
private:
    int t;
    string old_pin;
public:
    void type_of_service()  // To provide Available Services to User.
    {
        cout<<"Select the service you need:\n1.Debit\n2.Credit\n3.Check Balance\n4.Modify Pin\n";
        cin>>t;
        cout<<"Enter your pin number: ";
        cin>>old_pin;
        if(old_pin==pin)
        {
            switch(t)
            {
            case 1:
                debit();break;
            case 2:
                credit();break;
            case 3:
                check_balance();break;
            case 4:
                modify_pin();break;
            default:
                cout<<"Select available option.";
                break;
            }
        }
        else
            cout<<"Incorrect Pin.";
    }
};

void debit()                // To Withdraw money from Account.
{
    int cash;
    long long int temp_bal;
    string temp;
    cout<<"Enter the Amount you want to debit: ";
    cin>>cash;
    if(cash>balance)
        cout<<"Insufficient Amount\n";
    else if(cash>20000)
        cout<<"20,000 is the maximum amount that is allowed to withdraw\n";
    else if(cash<0)
        cout<<"Wrong input!!\n";
    else if(cash<500)
        cout<<"Amount should be greater than 500\n";
    else if(cash%50!=0)
        cout<<"Enter rounded values\n";
    else
    {
       temp_bal=balance-cash;
       operations.seekg(0,ios::beg);
       streampos endp;
       while(operations>>temp)
       {
           if(temp==pin)
           {
               endp = operations.tellg();
               clear_bal(endp,num_of_digits(balance));
               operations.seekp(endp,ios::beg);
               operations<<" "<<temp_bal;
               cout<<"Amount withdrawn!\n";
               cout<<"Thank You!";
               break;
           }
       }
       operations.clear();
    }
}
void credit()               // To Deposit the money into account.
{
    int cash;
    long long int temp_bal;
    string temp;
    cout<<"Enter the Amount you want to credit: ";
    cin>>cash;
    if(cash>30000)
        cout<<"30,000 is the maximum amount that is allowed to credit\n";
    else if(cash<0)
        cout<<"Wrong input!!\n";
    else
    {
       temp_bal=balance+cash;
       operations.seekg(0,ios::beg);
       while(operations>>temp)
       {
           if(temp==pin)
           {
               streampos endp = operations.tellg();
               operations.seekp(endp,ios::beg);
               operations<<" "<<temp_bal;
               cout<<"Amount Deposited\n";
               cout<<"Thank you!";
               break;
           }
       }
       operations.clear();
    }
}

void check_balance()        // To Display the balance of User.
{
    cout<<"Your balance in your account is: ";
    cout<<balance;
}

void modify_pin()           // To set new Pin for Old User.
{
    string mod_pin,temp;
    cout<<"Enter your new pin: ";
    cin>>mod_pin;
    if(mod_pin.length()!=4)
        cout<<"Provide Valid Pin!";
    else if(is_all_num(mod_pin)==1)
        cout<<"Enter digits only!";
    else
    {
        operations.seekg(0,ios::beg);
        while(operations>>temp)
       {
           if(temp==phone)
           {
               streampos endp = operations.tellg();     // We can write 'int' instead of 'streampos'
               operations.seekp(endp);
               operations<<" "<<mod_pin;
               cout<<"Pin Modified ";
               cout<<"Thank you!\n";
               break;
           }
       }
       operations.clear();
    }
}

class pin_num               // To set pin to new User.
{
public:
    void new_pin()
    {
        cout<<"Set your pin: ";
        cin>>pin;
        if(is_all_num(pin)==1)
        {
            cout<<"Enter only integers!";
        }
        else
        {
            if(pin.length()==4)
            {
                cout<<"You have successfully set your pin\n";
                cout<<"Enter your balance to add in data: ";
                cin>>balance;
                operations.seekp(0,ios::end);
                operations<<phone_t<<" "<<pin<<" "<<balance<<endl;
                cout<<"Your data has been stored. Thank You!";
            }
            else
                cout<<"Pin not valid.\n";
        }
    }
};

class user:public pin_num,public service    // After main function.
{
public:
    void starting()
    {
        int user_type;
        cout<<"Are you a new user?\n1.Yes\t2.No\n";
        cin>>user_type;
        switch(user_type)
        {
            case 1:         // For New User.
                cout<<"Enter your phone number: ";
                cin>>phone_t;
                if(is_all_num(phone_t)==1)
                    cout<<"Enter only integers!";
                else if(store(phone_t)==1)
                    cout<<"Number Already exists";
                else if(phone_t.length()==10)
                    new_pin();
                else
                    cout<<"Phone number must be 10 digits";
                break;
            case 2:         // For Old User.
                cout<<"Enter your phone number: ";
                cin>>phone;
                if(is_all_num(phone)==1)
                    cout<<"Enter only integers!";
                else if(phone.length()==10)
                {
                    if(store(phone)==1)
                        type_of_service();
                    else
                        cout<<"Phone number not found!";
                }
                else
                    cout<<"Phone number must be 10 digit.\n";
                break;
            default:
                cout<<"Select Available Option!";
                break;
        }
    }
};

int is_all_num(string str)  // to check whether the string has all numbers or not.
{
    for(int i=0;str[i]!='\0';i++)
    {
        if(str[i]>=48 && str[i]<=57)
            continue;
        else
            return 1;
    }
    return 0;
}

int store(string p)         // For storing the phone number,pin and amount, also to check whether the number exists or not.
{
    operations.seekg(0,ios::beg);
    while(!operations.eof())
    {
        operations>>phone;
        operations>>pin;
        operations>>balance;
        if(phone==p)
        {
            return 1;
        }
    }
    operations.clear();
    return 0;
}

int num_of_digits(long long int t)
{
    int c=0;
    while(t!=0)
    {
        t/=10;
        c++;
    }
    return c;
}

void clear_bal(streampos e,int c)
{
    for(int i=0;i<c+1;i++)
    {
        operations.seekp(e,ios::beg);
        operations<<" ";
        e=operations.tellg();
    }
}

int main()              // main function to start the program.
{
    if(operations.is_open())
    {
        user n;
        n.starting();
    }
    else
        cout<<"File unable to open\n";
}
