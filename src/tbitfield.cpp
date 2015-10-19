// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len<0) 
		throw "Negative Length";
	else
	{
		BitLen=len;
		MemLen = (BitLen + sizeof(TELEM) - 1) / (sizeof(TELEM));;
		pMem=new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
		pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	for(int i=0;i<MemLen;i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	if (pMem!=NULL)
	delete []pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
   return n/(sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1<<n%(sizeof(TELEM));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw "Negative Index";
	else if (n > BitLen)
		throw "Index more than length of bit field";
	else 
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw "Negative Index";
	else 
		if ( n > BitLen)
		throw "Index more than length of bit field";
		else
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  
	if (n <0)
		throw "Negative Index";
	else if (n >BitLen)
		throw "Index more than length of bit field";
	else 
		{
			int bit=0;
	if(((pMem[GetMemIndex(n)])&GetMemMask(n))>0)
		bit=1;
	return bit;
		}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
 
			BitLen = bf.BitLen;
			if (MemLen != bf.MemLen)
			{
				MemLen = bf.MemLen;
				if (pMem != NULL)
					delete []pMem;
				pMem = new TELEM[MemLen];
			}
				if (pMem != NULL)
				{
					for(int i=0;i<MemLen;i++)
					pMem[i]=bf.pMem[i];
				}
			
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
			int r=1;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < BitLen;i++)
		{
			if (GetBit(i)!=bf.GetBit(i))
			{
				r = 0;
				break;
			}
		}
	}
	else
	{
		r = 0;
	}
	return r;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (bf.BitLen !=BitLen )
		return 1;
  else
		{
			for(int i=0;i<MemLen;i++)
			if (pMem[i]!=bf.pMem[i]) 
				return 1;
			return 0;
		}
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int maxlen=bf.BitLen;
	if(BitLen>=bf.BitLen)
		maxlen=BitLen;
	TBitField bf2(maxlen);
	for(int i=0;i<MemLen;i++)
		bf2.pMem[i]=pMem[i];
	for(int i=0;i<bf.MemLen;i++)
		bf2.pMem[i]=bf2.pMem[i]|bf.pMem[i];

	return bf2;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxlen=bf.BitLen;

	if(BitLen>=bf.BitLen)
	{
		maxlen=BitLen;
		TBitField bf2(maxlen);
		for(int i=0;i<bf.MemLen;i++)
			bf2.pMem[i]=bf.pMem[i];
		for(int i=0;i<MemLen;i++)
			bf2.pMem[i]=pMem[i]&bf2.pMem[i];	
		return bf2;
	}
	else
	{
		TBitField bf2(maxlen);
		for(int i=0;i<MemLen;i++)
			bf2.pMem[i]=pMem[i];
		for(int i=0;i<bf.MemLen;i++)
			bf2.pMem[i]=bf.pMem[i] & bf2.pMem[i];	
		return bf2;
	}	
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField bf2(BitLen);
	int len=BitLen;
	TBitField bf2(len);
	for(int i=0;i<(MemLen-1);i++)
		bf2.pMem[i]=~pMem[i];

	int a = (MemLen-1)*sizeof(TELEM);
	for (int i = a; i < BitLen; i++)
	{
		if (this->GetBit(i) == 0)
			bf2.SetBit(i);
	}
	return bf2;
  
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  char c;
	int i = 0;
	istr >> c;
	while ((c == '1') || (c == '0'))
	{
		if (c == '1')
		{
			bf.SetBit(i);
		}
		if (c == '0') 
		{
			bf.ClrBit(i);
		}
		i++;
		istr >> c;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  ostr << "Length = " << bf.BitLen << endl;
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
