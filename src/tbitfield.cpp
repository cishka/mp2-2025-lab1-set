// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>
#include <stdexcept>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw std::out_of_range("the length is less than zero");
    }

    BitLen = len;

    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) {
        throw out_of_range("went abroud");
    }
    return n / (sizeof(TELEM) * 8);
    return FAKE_INT;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("went abroud");
    }
    return 1 << (n % (sizeof(TELEM) * 8));
    return FAKE_INT;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
    return FAKE_INT;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("went abroud");
    }

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("went abroud");
    }

    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw out_of_range("went abroud");
    }

    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[idx] & mask) != 0;
    return FAKE_INT;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
    return FAKE_BITFIELD;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
    return FAKE_INT;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return ~(*this == bf)%2;
    return FAKE_INT;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    int minMem = MemLen < bf.MemLen ? MemLen : bf.MemLen;

    for (int i = 0; i < minMem; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];

    if (MemLen > bf.MemLen)
    {
        for (int i = bf.MemLen; i < MemLen; i++)
            result.pMem[i] = pMem[i];
    }
    else if (bf.MemLen > MemLen)
    {
        for (int i = MemLen; i < bf.MemLen; i++)
            result.pMem[i] = bf.pMem[i];
    };
    return result;
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    // Для оставшихся элементов - нули (поскольку операция И)
    return result;
    return FAKE_BITFIELD;//понять нужны ли они вообще или нет
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);

    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];
   // if (MemLen > 1) {
        // Обнуляем лишние биты в последнем элементе
        int bitsInLastElem = BitLen % (sizeof(TELEM) * 8);
        if (bitsInLastElem != 0)
        {
            TELEM mask = (1 << bitsInLastElem) - 1;
            result.pMem[MemLen - 1] &= mask;
        }
    //}
        return result;
    return FAKE_BITFIELD;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        int bit;
        istr >> bit;
        if (bit == 1)
            bf.SetBit(i);
        else if (bit == 0)
            bf.ClrBit(i);
        else
            throw invalid_argument("Invalid input: expected 0 or 1");
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    ostr << "{ ";
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
        if (i < bf.BitLen - 1)
            ostr << ", ";
    }
    ostr << " }";
    return ostr;
}
