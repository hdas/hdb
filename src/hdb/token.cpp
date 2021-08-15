
#include "db.h"
#include "token.h"
#include "DVariable.h"

int TokenTest()
{
	char thestr[257];
	int res, nTokens, i = 0;
	//char ** tokens;
	//Query * firsttoken;
	DStack * pStack = NULL, *t = NULL;
	char * token;

	for (;;)
	{
		printf("Token>");
		fgets(thestr, 256, stdin);
		if (thestr[strlen(thestr) - 1] == '\n') thestr[strlen(thestr) - 1] = 0;

		pStack = CreateTokensEx(thestr, &nTokens, &res);

		token = CreateToken(thestr, 0);
		if (strcmp(token, "QUIT") == 0) break;
		free(token);
		i = 0;

		/*t=pStack->link;
		while(t!=NULL)
		{
		printf(">%c#%c#%s<\n", t->pVar->DataType, t->pVar->VarClass, t->pVar->strval);
		t=t->link;
		}*/

		for (;;)
		{
			token = CreateToken(thestr, i);
			if (strlen(token) == 0) break;
			printf("\n%ld : >%s<", i, token);
			i++;
			free(token);
		}

		printf("\n");
		delete pStack;
	}
	return true;
}


char * CreateToken(char * t, int tokennumber)
{
	char * tkn;
	int tn = 0, ti = 0, i, lt;
	char new_token, string_started = false;

	tkn = (char*)malloc(256 * sizeof(char));
	tkn[0] = 0x00;

	// go through the whole command character by character
	lt = strlen(t);
	for (i = 0; i < lt; i++)
	{
		new_token = false;
		// Looking for character which may start a new token
		if (t[i] == 39)
		{
			if (string_started)
			{
				if (t[i + 1] == 39) i++; else string_started = false;
			}
			else
			{
				string_started = true;
				new_token = true;
			}
		}
		else
		{
			if (!string_started)
			{
				if (isDBwhitespacechar(t[i]) || isDBspecialchar(t[i]))
				{
					if ((i > 0) && (!isDBwhitespacechar(t[i - 1]))) new_token = true;
				}
				else
				{
					if ((i > 0) && isDBspecialchar(t[i - 1])) new_token = true;
				}
				// Packing "<>" , "<=" , ">=" oparators
				if ((i > 0) && (t[i] == '>' || t[i] == '=')) if (t[i - 1] == '<' || t[i - 1] == '>') new_token = false;
				// Packing "||" oparator
				if (i > 0 && t[i] == '|') if (t[i - 1] == '|') new_token = false;

			}
		}

		// Increase the tn  Its the second phase checking !!!!! 
		if (new_token)
		{
			tn++;
			if (i > 0)
			{
				if (t[i] == 39 && isDBwhitespacechar(t[i - 1])) tn--;
			}
			else
			{
				if (t[i] == 39) tn--;
			}
		}

		// If token(tokennumber) already got then break the loop
		if (tn > tokennumber) break;


		// Now inserting the character into the token

		if ((tokennumber == tn) && ((!isDBwhitespacechar(t[i])) || string_started))
		{
			tkn[ti++] = ((!string_started) && (t[i] > 96) && (t[i] < 123)) ? t[i] - 32 : t[i]; // case covert or not
		}
	}

	tkn[ti] = 0x00;
	//if(strlen(tkn)==0) return NULL;
	return tkn;
}



char isDBspecialchar(char c)
{
	char spcch[] = { '(', ')', '*', '+', '-', '/', ',', ';', '<', '>', '=', '|',
		'%', '&', '@', '!', 0x00 };
	char sp = FALSE;
	int i;
	//if(c=='(' || c==')' || c=='*' || c=='+' || c=='-' || c=='/' || c==',' || c==';' || c=='<' || c=='>' || c=='=' || c=='|' || c=='%' || c=='&' ||c=='@'||c=='!')
	//	return true; else return false;
	for (i = 0; spcch[i] != 0; i++) if (c == spcch[i]) { sp = TRUE; break; }

	return sp;
}

char isDBnormalchar(char c)
{
	if (c == '#' || c == '_' || c == '$' || c == '.') return true; else return false;
}

char isDBwhitespacechar(char c)
{
	if (c == 32 || c == 9 || c == 10 || c == 13) return true; else return false;
}






//********************
// The following function is
// Under developement
//********************
DStack * CreateTokensEx(char * t, int * pnTokens, int * pRetcd)
{
	char * tkn;
	int tn = 0, ti = 0, i, lt, res, desired_tokennumber = 0;
	char new_token, string_started = false;
	DVariable * pVar = NULL;
	DStack * pStack = new DStack();

	*pnTokens = 0;

	tkn = (char*)malloc(256 * sizeof(char));
	tkn[0] = 0x00;

	// go through the whole command character by character
	lt = strlen(t);
	for (i = 0; i < lt; i++)
	{
		new_token = false;
		// Looking for character which may start a new token
		if (t[i] == 39)
		{
			if (string_started)
			{
				if (t[i + 1] == 39) i++; else string_started = false;
			}
			else
			{
				string_started = true;
				new_token = true;
			}
		}
		else
		{
			if (!string_started)
			{
				if (isDBwhitespacechar(t[i]) || isDBspecialchar(t[i]))
				{
					if ((i > 0) && (!isDBwhitespacechar(t[i - 1]))) new_token = true;
				}
				else
				{
					if ((i > 0) && isDBspecialchar(t[i - 1])) new_token = true;
				}
				// Packing "<>" , "<=" , ">=" oparators
				if ((i > 0) && (t[i] == '>' || t[i] == '=')) if (t[i - 1] == '<' || t[i - 1] == '>') new_token = false;
				// Packing "||" oparator
				if (i > 0 && t[i] == '|') if (t[i - 1] == '|') new_token = false;

			}
		}

		// Increase the tn  Its the second phase checking !!!!! 
		if (new_token)
		{
			tn++;
			if (i > 0)
			{
				if (t[i] == 39 && isDBwhitespacechar(t[i - 1])) tn--;
			}
			else
			{
				if (t[i] == 39) tn--;
			}
		}

		// If token(tokennumber) already got then break the loop
		if (tn > desired_tokennumber)
		{
			int lll = 0;

			tkn[ti] = 0x00; // terminating

			// Now storing the token
			pVar = new DVariable();
			lll = strlen(tkn);
			if (tkn[0] == 39) // i.e. it is a string value
			{
				pVar->m_strval = (char*)malloc(lll*sizeof(char));
				tkn[lll - 1] = 0x00; // trim ending ' char
				strcpy(pVar->m_strval, (tkn + 1));
				pVar->m_VarClass = VarClass::Value;
				pVar->m_DataType = DT_CHAR;

			}
			else if (tkn[0] >= 48 && tkn[0] <= 57) //i.e. it is a number value
			{
				pVar->m_VarClass = VarClass::Value;
				pVar->m_DataType = DT_NUMBER;
				pVar->m_numval = atof(tkn);
			}
			else
			{
				char tmpcc = false;
				char * ppstr = NULL;

				// first initialize the else condition; any field name or parm etc
				pVar->m_VarClass = VarClass::Param;
				pVar->m_strval = (char*)malloc(lll*sizeof(char));
				strcpy(pVar->m_strval, tkn);


				tmpcc = dGetOparatorCode(tkn);

				if (tmpcc)
				{
					pVar->m_VarClass = VarClass::Operator;
					pVar->m_DataType = tmpcc;
				}

				tmpcc = dGetDataTypeCode(tkn);
				if (tmpcc) // Any valid reserved word
				{
					pVar->m_VarClass = VarClass::Datatype;
					pVar->m_DataType = tmpcc;
				}

				tmpcc = dGetAggrCode(tkn);
				if (tmpcc) // Any valid reserved word
				{
					pVar->m_VarClass = VarClass::Aggregate;
					pVar->m_DataType = tmpcc;
				}

				tmpcc = dGetFunctionCode(tkn);
				if (tmpcc) // Any valid reserved word
				{
					pVar->m_VarClass = VarClass::Function;
					pVar->m_DataType = tmpcc;
				}

				tmpcc = dGetReserveWordCode(tkn);
				if (tmpcc) // Any valid reserved word
				{
					pVar->m_VarClass = VarClass::ReservedWord;
					pVar->m_DataType = tmpcc;
				}
			}

			res = pStack->Push(pVar, FALSE);
			//dDestroyVariable(pVar);

			//break;
			// Now initializing variables for fetching next token
			ti = 0;
			desired_tokennumber++;
		}
		else // Now inserting the character into the token
		{
			if ((desired_tokennumber == tn) && ((!isDBwhitespacechar(t[i])) || string_started))
			{
				tkn[ti++] = ((!string_started) && (t[i] > 96) && (t[i] < 123)) ? t[i] - 32 : t[i]; // case covert or not
			}
		}
	}

	//tkn[ti]=0x00;
	//if(strlen(tkn)==0) return NULL;
	*pnTokens = tn;
	return pStack;
}



