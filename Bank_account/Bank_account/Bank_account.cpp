// Bank_account.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdio> 

using namespace std;

class Bank
{
    bool blocking = false;
    const int percent = 5;
    unsigned int credit;
    int Lchance;
    string username;
    string password;
    string latest_pay;
    const string admin_code = "AdmCode";

    void Accout_creater();
    void Percent();
    void Get_info();
    void Blocker();
    void Pay();
    void Set_new_latest_pay();
    void Remove();
    void End();
    void Pas_change();
    bool Accout_checker();
    bool Logining();
    
public:
    Bank()
    {
        cout << "Здравствуйте. Для начала работы введите ваш login?\n";
        cin >> username;
        if (Accout_checker())
        {
            Accout_creater();
            cout << "Аккаунт успешно создан.\n";
        }
        else
        {

            Get_info();

            while (!Logining() && !blocking)
            {
                Lchance--;
                if (Lchance <= 0)
                    blocking = 1;
            }

            if (!blocking)
            {
                Percent();
                Pay();

                if (credit <= 0)
                {
                    cout << "Поздравляем! Вся задолженность оплачена.\n";
                    Remove();
                }
                else
                    End();
            }
            else
            {
                ofstream set(username + ".txt");

                Blocker();

                set << password << "\n";
                set << credit << "\n";
                set << latest_pay << "\n";
                set << "1\n";

                set.close();
            }
        }
    }
};
bool Bank::Accout_checker()
{
    ifstream check;
    check.open(username + ".txt");
    if (check.is_open())
    { 
        check.close();
        return 0;
    }
    else
    {
        cout << "Вы не состоите в списке тех, кто брали займ\n";
        return 1;
    }
}
void Bank::Accout_creater()
{
    ofstream create(username + ".txt");
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);

    cout << "Введите пароль, который вы хотели бы установить\n";
    for (bool i = 1; i == 1;)
    {
        
        cin >> password;

        if (password != "BLOCK")
            i = 0;
        else
            cout << "Введите другой пароль\n";
    }

    bool cn = 0;
    cout << "Введите начальную сумму займа\n";
    do 
    {
        cn = 0;
        cin >> credit;
        if (!cin)
        {
            cn = 1;
            cin.clear();
            cin.ignore(100, '\n');
            cout<<"Вы должны ввести число. Без точек, запятых и других подобных символов\n";
        }
    } while (cn);

    create << password << "\n";
    create << credit << "\n";
    if((now->tm_mday < 10)&&(now->tm_mon+1 < 10))
    {
        create << (now->tm_year + 1900) << ".0"
            << (now->tm_mon + 1) << ".0"
            << now->tm_mday
            << "\n";
    }
    else if (now->tm_mday < 10)
    {
        create << (now->tm_year + 1900) << '.'
            << (now->tm_mon + 1) << ".0"
            << now->tm_mday
            << "\n";
    }
    else if (now->tm_mon + 1 < 10)
    {
        create << (now->tm_year + 1900) << ".0"
            << (now->tm_mon + 1) << '.'
            << now->tm_mday
            << "\n";
    }
    else
    {
        create << (now->tm_year + 1900) << '.'
            << (now->tm_mon + 1) << '.'
            << now->tm_mday
            << "\n";
    }
    create << "10\n";

    create.close();
}
void Bank::Get_info()
{
    ifstream get;
    get.open(username + ".txt");
    string for_credit; string for_Lchance;

    getline(get, password);
    getline(get, for_credit);
    getline(get, latest_pay);
    getline(get, for_Lchance);

    credit = stoi(for_credit);
    Lchance = stoi(for_Lchance);

    get.close();
}
void Bank::Blocker()
{
    password = "BLOCK";

}
void Bank::Percent()
{
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);

    int year = now->tm_year + 1900;
    int mon = now->tm_mon + 1;
    int day = now->tm_mday;

    int latest_pay_year = 0;
    for (size_t i = 0; i < 4; i++)
    {
        latest_pay_year *= 10;
        latest_pay_year += latest_pay[i] - '0';
    }
    
    int latest_pay_mon = 0;
    for (size_t i = 5; i < 7; i++)
    {
        latest_pay_mon *= 10;
        latest_pay_mon += latest_pay[i] - '0';
    }
    
    int latest_pay_day = 0;
    for (size_t i = 8; i < 10; i++)
    {
        latest_pay_day *= 10;
        latest_pay_day += latest_pay[i] - '0';
    }

    year -= latest_pay_year;
    mon -= latest_pay_mon;
    day -= latest_pay_day;
    
    day += year * 365;
    
    int d_30 = 0;
    int d_31 = 0;
    bool feb = 0;
    bool plus = mon>=latest_pay_day;

    if (plus)
    {
            while (mon != latest_pay_mon)
            {
        
                if ((latest_pay_mon == 1) || (latest_pay_mon == 3) || (latest_pay_mon == 5) || (latest_pay_mon == 7) || (latest_pay_mon == 8) || (latest_pay_mon == 10) || (latest_pay_mon == 12))
                    d_31++;
                else if (latest_pay_mon == 2)
                    feb = 1;
                else
                    d_30++;
                latest_pay_mon++;
            }
    }
    else
    {
        while (mon != latest_pay_mon)
        {

            if ((mon == 1) || (mon == 3) || (mon == 5) || (mon == 7) || (mon == 8) || (mon == 10) || (mon == 12))
                d_31++;
            else if (mon == 2)
                feb = 1;
            else
                d_30++;
            mon++;
        }
    }

    if (plus)
    {
        day += d_30 * 30;    day += d_31 * 31;
        if (feb)
            day += 28;
    }
    else
    {
        day -= d_30 * 30;    day -= d_31 * 31;
        if (feb)
            day -= 28;
    }

    if (day >= 30)
    {
        day /= 30;

        for (; day > 0; day--)
            credit += (credit / 100) * 5;
    }
}
void Bank::Pay()
{
    unsigned int pay;
    string confirm;

    cout << "Введите сумму платежа, которую хоте бы внести.\n";
    
    bool cn;
        do
        {
            cn = 0;
            cin >> pay;
            if (!cin)
            {
                cn = 1;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Введите целое ЧИСЛО. Если Вы не хотите платить, то введите '0'\n";
            }
        } while (cn);

    cout << "Код подтверждения от администратора:.\n";
    cin >> confirm;
    if (confirm == "stop")
        End();

    if (confirm == admin_code)
    {
        credit -= pay;
        Lchance = 10;
        Set_new_latest_pay();
        cout << "Отлично. Платёж выполнен успешно.\n";
    }
    else
        cout << "Код подтверждения не подходит, отмена операции.\n";
}
void Bank::Set_new_latest_pay()
{
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    if ((now->tm_mday < 10) && (now->tm_mon + 1 < 10))
    {
        latest_pay = to_string(now->tm_year + 1900) + ".0"
            + to_string(now->tm_mon + 1) + ".0"
            + to_string(now->tm_mday);
    }
    else if (now->tm_mday < 10)
    {
        latest_pay = to_string(now->tm_year + 1900) + '.'
            + to_string(now->tm_mon + 1) + ".0"
            + to_string(now->tm_mday);
    }
    else if (now->tm_mon < 10)
    {
        latest_pay = to_string(now->tm_year + 1900) + ".0"
            + to_string(now->tm_mon + 1) + "."
            + to_string(now->tm_mday);
    }
    else
    {
        latest_pay = to_string(now->tm_year + 1900) + '.'
            + to_string(now->tm_mon + 1) + '.'
            + to_string(now->tm_mday);
    }
    
}
void Bank::Remove()
{
    username = username + ".txt";
    remove(username.c_str());
}
void Bank::End()
{
    ofstream reset(username + ".txt", ios_base::trunc);
    reset << password << "\n";
    reset << credit << "\n";
    reset << latest_pay << "\n";
    reset << Lchance;

    reset.close();
}
void Bank::Pas_change()
{
    cout << "Введите новый пароль для аккаунта.\n";
    for (bool i = 1; i == 1;)
    {
        cin >> password;

        if (password != "BLOCK")
            i = 0;
        else
            cout << "Введите другой пароль\n";
    }
}
bool Bank::Logining()
{
    string log_try;
    if (password != "BLOCK")
    {
        cout << "Введите пароль к вашему аккаунту\n";
        cin >> log_try;
        if ((log_try == admin_code) || (log_try == password))
            return 1;
        else
        {
            cout << "Ошибка!.\n";
            if (Lchance == 2 )
                cout << "У вас осталась одна попытка для ввода пароля.\n";
            return 0;
        }
    }
    else
    {
        cout << "Ваш аккаунт заблокирован. Для восстановления аккаунта введите пароль администратора.\n";
        cin >> log_try;
        if ((log_try == admin_code))
        {
            Pas_change();
            return 1;
        }  
        else
        {
            cout << "Ошибка!.\n";
            return 0;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    Bank bank;
}


