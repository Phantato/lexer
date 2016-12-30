#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
using namespace std;

int charencode(char ch)
{
    if(ch>='a' && ch <= 'z')
    {
        return ch-'a';
    }
    else if(ch >= 'A' && ch <= 'Z')
    {
        return 25 + ch - 'A';
    }
    else if(ch >= '0' && ch <= '9')
    {
        return 51 + ch - '0';
    }
    else if(ch == '_')
    {
        return 62;
    }
    else
    {
        return 63;
    }
}
tuple<int, string> character(ifstream &fin, int &row, int &col)
{
	char ch;
    int stat=0;
	tuple<int, string> tup;
    int DFA[15][63], i, j;
    for(i = 0; i < 15; i++)
    {
        for(j = 0; j < 63; j++)
        {
            DFA[i][j] = 14;
        }
    }
    DFA[0]['i'-'a'] = 1;
    DFA[1]['f'-'a'] = 2;
    DFA[1]['n'-'a'] = 3;
    DFA[3]['t'-'a'] = 4;
    DFA[0]['w'-'a'] = 5;
    DFA[5]['h'-'a'] = 6;
    DFA[6]['i'-'a'] = 7;
    DFA[7]['l'-'a'] = 8;
    DFA[8]['e'-'a'] = 9;
    DFA[0]['e'-'a'] = 10;
    DFA[10]['l'-'a'] = 11;
    DFA[11]['s'-'a'] = 12;
    DFA[12]['e'-'a'] = 13;
	get<1>(tup).clear();
	ch = fin.get();
    while(fin.good() && (i = charencode(ch)) != 63)
    {
        stat = DFA[stat][i];
        get<1>(tup) += ch;
    	ch = fin.get();
        col++;
    }
    switch (stat)
    {
        case 2:
            get<0>(tup) = 31;
        	get<1>(tup).clear();
            break;
        case 4:
            get<0>(tup) = 32;
            get<1>(tup).clear();
            break;
        case 9:
            get<0>(tup) = 33;
            get<1>(tup).clear();
            break;
        case 13:
            get<0>(tup) = 34;
            get<1>(tup).clear();
            break;
        default :
            get<0>(tup) = 0;
    }
    return tup;
}

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
	else if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
	// lable
	{
        fin.seekg(-1, fin.cur);
        tup = character(fin, row, col);
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
    ofstream fout;
    int row, col;
    // character(fin,row,col);
    // return 0;
	row = 1;
	col = 1;
	tuple<int, string> tup;
    cout<<"Open File:"<<endl;
	cin>>filename;
    fin.open(filename);
    if(fin.fail())
    {
        cout<<"Unable to Open this File!"<<endl;
        return 0;
    }
    cout<<"Print to File:"<<endl;
    cin>>filename;
    fout.open(filename);
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
		cout<<'<'<<get<0>(tup)<<','<<get<1>(tup)<<">\n";
        fout<<'<'<<get<0>(tup)<<','<<get<1>(tup)<<'>';
		tup = lexer(fin, row, col);
        if(get<0>(tup) == -2)
        {
            cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
            fout<<"\n!\n";
        }
        while(fin.good() && (get<0>(tup) == -1 || get<0>(tup) == -2))
        {
    		tup = lexer(fin, row, col);
            if(get<0>(tup) == -2)
            {
                cout<<"Error at "<<filename<<' '<<row<<':'<<col<<endl;
                fout<<"\n!\n";
            }
        }
	}
    fout.close();
	return 0;
}
