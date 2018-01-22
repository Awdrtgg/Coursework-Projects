#include "library.h"

Time::Time()
{
    ifstream ft(".\\data\\Time.txt");
    if (ft.fail()) {
        this->time = 0;
    }
    string s;
    ft >> s;
    ft.close();
    this->time = stoi(s);
}

Time::Time(int time)
{
    this->time = time;
}

int Time::month()
{
    int d = time % 365;
    if (d < 31)	return 1;
    else if (d < 59) return 2;
    else if (d < 90) return 3;
    else if (d < 120) return 4;
    else if (d < 151) return 5;
    else if (d < 181) return 6;
    else if (d < 212) return 7;
    else if (d < 243) return 8;
    else if (d < 273) return 9;
    else if (d < 304) return 10;
    else if (d < 334) return 11;
    else return 12;
}

int Time::day()
{
    int d = time % 365 + 1;
    switch (this->month())
    {
    case 1: return d; break;
    case 2: return (d - 31); break;
    case 3: return (d - 59); break;
    case 4: return (d - 90); break;
    case 5: return (d - 120); break;
    case 6: return (d - 151); break;
    case 7: return (d - 181); break;
    case 8: return (d - 212); break;
    case 9: return (d - 243); break;
    case 10: return (d - 273); break;
    case 11: return (d - 304); break;
    case 12: return (d - 334); break;

    default:
        return FAIL;
        break;
    }
}

string Time::date()
{
    stringstream s;
    s << this->year_real() << "/" << this->month() << "/" << this->day();

    return s.str();
}

void Time::NextDay()
{
    this->time += 1;
    l->CheckReserve();
    this->Save();
}

void Time::Save()
{
    ofstream ft(".\\data\\Time.txt");
    ft << to_string(this->time);
    ft.close();
}
