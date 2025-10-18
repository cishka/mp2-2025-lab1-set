// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    if (mp <= 0) {
        throw "invalid size";
    }
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
    //return FAKE_INT;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (0 > Elem || Elem >= MaxPower) {
        throw std::invalid_argument("Elem vne size");
    }
    return BitField.GetBit(Elem);
    //return FAKE_INT;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (0 > Elem || Elem >= MaxPower) {
        throw std::invalid_argument("Elem vne size");
    }
    return BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (0 > Elem || Elem >= MaxPower) {
        throw std::invalid_argument("Elem vne size");
    }
    return BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
    //return FAKE_SET;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower == s.MaxPower) {
        if (BitField == s.BitField) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
   //return FAKE_INT;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (MaxPower == s.MaxPower) {
        if (BitField == s.BitField) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
    //return FAKE_INT;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int newMaxPower = std::max(MaxPower, s.MaxPower);
    TSet result(newMaxPower);
    result.BitField = BitField | s.BitField;
    return result;
    //return FAKE_SET;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0) {
        throw std::invalid_argument("Elem>0!!");
    }
    int newMaxPower = std::max(MaxPower, Elem + 1);
    TSet result(newMaxPower);
    result.BitField = BitField;
    result.InsElem(Elem);
    return result;
    //return FAKE_SET;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
    //return FAKE_SET;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int newMaxPower = std::max(MaxPower, s.MaxPower);
    TSet result(newMaxPower);
    result.BitField = BitField & s.BitField;
    return result;
    //return FAKE_SET;
}

TSet TSet::operator~(void) // дополнение
{
    TSet newTset(MaxPower);
    newTset.BitField = ~BitField;
    return newTset;
    //return FAKE_SET;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{// Вводим мощность множества
    int power;
    istr >> power;

    // Создаем временное множество нужной мощности
    TSet temp(power);

    // Вводим элементы множества до -1
    int elem;
    while (true)
    {
        istr >> elem;
        if (elem == -1) // признак конца ввода
            break;
        if (elem < 0 || elem >= temp.GetMaxPower())
            throw "Element out of range";
        temp.InsElem(elem);
    }

    s = temp;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.GetMaxPower(); i++)  // Используем GetMaxPower() вместо прямого доступа
    {
        if (s.IsMember(i))
        {
            if (!first)
                ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << " } (MaxPower: " << s.GetMaxPower() << ")";
    return ostr;
}
