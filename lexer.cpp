#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
using namespace std;

tuple<int, string> lexer(ifstream &fin, int &row, int &col)
{
	char ch;
	tuple<int, string> tup;
	get<0>(tup) = -1;
	get<1>(tup).clear();
	ch = fin.get();
	col++;
	if(ch == '#' || ch == '\n')
	{
		while(ch != '\n')
		{
			ch = fin.get();
			col++;
		}
		row++;
		col = 1;
	}
	else if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	// lable
	{
		get<0>(tup) = 0;
		while(fin.good() && (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
		{
			get<1>(tup) += ch;
			ch = fin.get();
			col++;
		}
		fin.seekg(-1, fin.cur);
		col--;
	}
	else if((ch >= '0' && ch <= '9'))
	// integer
	{
		get<0>(tup) = 1;
		while(fin.good() && (ch >= '0' && ch <= '9'))
		{
			get<1>(tup) += ch;
			ch = fin.get();
			col++;
		}
		fin.seekg(-1, fin.cur);
		col--;
	}
	else if(ch == '\'')
	// character
	{
		get<0>(tup) = 2;
		ch = fin.get();
		col++;
		get<1>(tup) += ch;
		if(ch == '\\')
		{
			ch = fin.get();
			col++;
			if(fin.good())
			{
				get<1>(tup) += ch;
			}
			else
			// ERROR!
			{
                get<0>(tup) = -2;
			}
		}
		ch = fin.get();
		col++;
		if(ch != '\'')
		// ERROR!
		{
			fin.seekg(-1, fin.cur);
			col--;
            get<0>(tup) = -2;
		}
	}
	else if(ch == '\"')
	// string
	{
		get<0>(tup) = 3;
		ch = fin.get();
		col++;
		while(fin.good() && ch != '\"')
		{
			get<1>(tup) += ch;
			if(ch == '\\')
			{
				ch = fin.get();
				col++;
				if(fin.good())
				{
					get<1>(tup) += ch;
				}
				else
				// ERROR!
				{
					get<0>(tup) = -2;
				}
			}
			ch = fin.get();
			col++;
		}
        if(!fin.good())
        {
            get<0>(tup) = -2;
        }
	}
	else if(ch == '=')
	// equal
	{
		ch = fin.get();
		col++;
		if(ch == '=')
		{
			get<0>(tup) = 5;
		}
		else
		{
			get<0>(tup) = 4;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '>')
	// greater
	{
		ch = fin.get();
		col++;
		if(ch == '=')
		{
			get<0>(tup) = 6;
		}
		else if(ch == '>')
		{
			get<0>(tup) = 21;
		}
		else
		{
			get<0>(tup) = 7;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '<')
	// less
	{
		ch = fin.get();
		col++;
		if(ch == '=')
		{
			get<0>(tup) = 8;
		}
		else if(ch == '<')
		{
			get<0>(tup) = 20;
		}
		else
		{
			get<0>(tup) = 9;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '!')
	// not
	{
		ch = fin.get();
		col++;
		if(ch == '=')
		// not equal
		{
			get<0>(tup) = 10;
		}
		else
		{
            get<0>(tup) = 17;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '+')
	// add
	{
		ch = fin.get();
		col++;
		if(ch == '+')
		// self-add
		{
			get<0>(tup) = 18;
		}
		else
		{
            get<0>(tup) = 11;
			fin.seekg(-1, fin.cur);
			col--;
        }
	}
	else if(ch == '-')
	// minus
	{
		ch = fin.get();
		col++;
		if(ch == '-')
		// self-minus
		{
			get<0>(tup) = 19;
		}
		else if((ch >= '0' && ch <= '9'))
		// integer
		{
			get<0>(tup) = 1;
			get<1>(tup) += '-';
			while(fin.good() && (ch >= '0' && ch <= '9'))
			{
				get<1>(tup) += ch;
				ch = fin.get();
				col++;
			}
			fin.seekg(-1, fin.cur);
			col--;
		}
		else
		{
            get<0>(tup) = 12;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '*')
	// multiply
	{
		get<0>(tup) = 13;
		ch = fin.get();
		col++;
	}
	else if(ch == '/')
	// devide
	{
		ch = fin.get();
		col++;
		if(ch == '/')
		// comment
		{
			while(ch != '\n')
			{
				ch = fin.get();
			}
            row++;
            col = 1;
		}
		else
		{
            get<0>(tup) = 10;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '&')
	// and
	{
		ch = fin.get();
		col++;
		if(ch == '&')
		{
			get<0>(tup) = 15;
		}
		else
		{
            get<0>(tup) = 22;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == '|')
	// or
	{
		ch = fin.get();
		col++;
		if(ch == '|')
		{
			get<0>(tup) = 16;
		}
		else
		{
            get<0>(tup) = 23;
			fin.seekg(-1, fin.cur);
			col--;
		}
	}
	else if(ch == ';')
	{
		get<0>(tup) = 24;
	}
	else if(ch == '(' )
	{
		get<0>(tup) = 25;
	}
	else if(ch == ')')
	{
		get<0>(tup) = 26;
	}
	else if(ch == '{' )
	{
		get<0>(tup) = 27;
	}
	else if(ch == '}')
	{
		get<0>(tup) = 28;
	}
    else if(ch == '[')
	{
		get<0>(tup) = 29;
	}
	else if(ch == ']')
	{
		get<0>(tup) = 30;
	}
	return tup;
}


int main()
{
	string filename;
	ifstream fin;
	int row, col;
	row = 1;
	col = 1;
	tuple<int, string> tup;
	cin>>filename;
	fin.open(filename);
	tup = lexer(fin, row, col);
    if(get<0>(tup) == -2)
    {
        cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
    }
	while(fin.good() && (get<0>(tup) == -1 || get<0>(tup) == -2))
	{
        tup = lexer(fin, row, col);
        if(get<0>(tup) == -2)
        {
            cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
        }
	}
	while(fin.good())
	{
		cout<<get<0>(tup)<<"->"<<get<1>(tup)<<'\n';
		tup = lexer(fin, row, col);
        if(get<0>(tup) == -2)
        {
            cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
        }
        while(fin.good() && (get<0>(tup) == -1 || get<0>(tup) == -2))
        {
    		tup = lexer(fin, row, col);
            if(get<0>(tup) == -2)
            {
                cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
            }
        }
	}
	return 0;
}
