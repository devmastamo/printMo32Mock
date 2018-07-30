#include "GCodeParse.h"
#include "Commands.h"

static bool AddToNum(char * rpParseNum, char rcNumCandidate)
{
    if((rcNumCandidate <= '9' && rcNumCandidate >= '0') || rcNumCandidate == '.' || rcNumCandidate == '-')
    {
        *rpParseNum = rcNumCandidate;
        return true;
    }
    else
    {
        return false;
    }
}

static bool SetFieldValue(char *rpField, float rfFieldValue, Command * rpCommand)
{
    switch(*rpField)
    {
        case 'S':
            rpCommand->fSField = rfFieldValue;
            break;
        case 'P':
            rpCommand->fPField = rfFieldValue;
            break;
        case 'X':
            rpCommand->fXField = rfFieldValue;
            break;
        case 'Y':
            rpCommand->fYField = rfFieldValue;
            break;
        case 'Z':
            rpCommand->fZField = rfFieldValue;
            break;
        case 'I':
            rpCommand->fIField = rfFieldValue;
            break;
        case 'J':
            rpCommand->fJField = rfFieldValue;
            break;
        case 'D':
            rpCommand->fDField = rfFieldValue;
            break;
        case 'H':
            rpCommand->fHField = rfFieldValue;
            break;
        case 'F':
            rpCommand->fFField = rfFieldValue;
            break;
        case 'R':
            rpCommand->fRField = rfFieldValue;
            break;
        case 'Q':
            rpCommand->fQField = rfFieldValue;
            break;
        case 'E':
            rpCommand->fEField = rfFieldValue;
            break;
        case 'N':
            rpCommand->fNField = rfFieldValue;
            break;
        default:
            return false;
            break;
    }
    *rpField = ' ';
    return true;
}

// A user should check rpCommand. If it's NULL, then we either need to read, or we're at end of file
eParseReturn ParseNextInstruction(char * rpCharString, size_t rdSize, size_t * rdIterStart, Command * rpCommand, char * rpField, char * rpParseNum, size_t * rpParseNumIndex, eParseState * tParseState)
{
    size_t dIterator;
    bool bDone = false;

    for(dIterator = *rdIterStart; dIterator < rdSize; dIterator++)
    {
        switch(*tParseState)
        {
            case PARSE_STATE_START:     // Basically just searches for newlines
                if(bDone)
                {
                    *rdIterStart = dIterator;
                    return PARSE_RETURN_DONE;
                }

                if(rpCharString[dIterator] == '\n')
                {
                    *tParseState = PARSE_STATE_PARSE_COMMAND;
                }
                break;
            // After a newline, look for a command code. If it's not either command code G or M, or a newline 
            //  (indicating another search for command code), go back to discarding characters for a newline.
            case PARSE_STATE_PARSE_COMMAND:     
                if(rpCharString[dIterator] == 'G')
                {
                    memset(rpCommand, 0, sizeof(Command));
                    rpCommand->tGCommand = G_COMMAND;
                    *tParseState = PARSE_STATE_PARSE_COMMAND_NUM;
                }
                else if(rpCharString[dIterator] == 'M')
                {
                    memset(rpCommand, 0, sizeof(Command));
                    rpCommand->tGCommand = M_COMMAND;
                    *tParseState = PARSE_STATE_PARSE_COMMAND_NUM;
                }
                else if(rpCharString[dIterator] != '\n')    // If it isn't a G or M command (or unexpected character, keep searching for a newline)
                {
                    *tParseState = PARSE_STATE_START;
                }
                break;
            // After a command code, look for a number and 'add' it to the current number.
            case PARSE_STATE_PARSE_COMMAND_NUM:
                if(rpCharString[dIterator] == ' ')
                {
                    if(*rpParseNumIndex)
                    {
                        rpCommand->dCommandNumber = atoi(rpParseNum);
                        memset(rpParseNum,0, MAX_FLOAT_DIGITS);
                        *rpParseNumIndex = 0;
                        *tParseState = PARSE_STATE_PARSE_ARGUMENT;
                    }
                    else
                    {
                        *tParseState = PARSE_STATE_START;
                        return PARSE_RETURN_ERROR;
                    }
                }
                else if(rpCharString[dIterator] == '\n')
                {
                    if(*rpParseNumIndex)
                    {
                        rpCommand->dCommandNumber = atoi(rpParseNum);
                        memset(rpParseNum,0, MAX_FLOAT_DIGITS);
                        *rpParseNumIndex = 0;
                        *tParseState = PARSE_STATE_PARSE_COMMAND;
                        *rdIterStart = dIterator;
                        return PARSE_RETURN_DONE;
                    }
                }
                else
                {
                    if(!AddToNum(&(rpParseNum[*rpParseNumIndex]),rpCharString[dIterator]))
                    {
                        *tParseState = PARSE_STATE_START;
                        return PARSE_RETURN_ERROR;
                    }
                    else
                    {
                        (*rpParseNumIndex)++;
                    }
                }
                break;
            case PARSE_STATE_PARSE_ARGUMENT:
                *rpField = ' ';
                switch(rpCharString[dIterator])
                {
                    case 'S':
                    case 'P':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'I':
                    case 'J':
                    case 'D':
                    case 'H':
                    case 'F':
                    case 'R':
                    case 'Q':
                    case 'E':
                    case 'N':
                        *rpField = rpCharString[dIterator];
                        *tParseState = PARSE_STATE_PARSE_ARGUMENT_NUM;
                        break;
                    case ';':
                    case ' ':
                        *tParseState = PARSE_STATE_START;
                        *rdIterStart = dIterator;
                        return PARSE_RETURN_DONE;
                    default:
                        *tParseState = PARSE_STATE_START;
                        return PARSE_RETURN_ERROR;
                        break;
                }
                break;
            case PARSE_STATE_PARSE_ARGUMENT_NUM:
                // When we're parsing through numbers and we encounter a space, set the field,
                //      If it's a valid field it has been set and we go to parse a new argument. Otherwise, return an error.
                //      If we never filled any numbers, it's an error too.
                if(rpCharString[dIterator] == ' ')
                {
                    if(*rpParseNumIndex)
                    {
                        if(!SetFieldValue(rpField, atof(rpParseNum), rpCommand))
                        {
                            *tParseState = PARSE_STATE_START;
                            return PARSE_RETURN_ERROR;
                        }
                        memset(rpParseNum,0, MAX_FLOAT_DIGITS);
                        *rpParseNumIndex = 0;
                        *tParseState = PARSE_STATE_PARSE_ARGUMENT;
                    }
                    else
                    {
                        *tParseState = PARSE_STATE_START;
                        return PARSE_RETURN_ERROR;
                    }
                }
                // When we're parsing through numbers and we encounter a newlinew, set the field,
                //      If it's a valid field it has been set and we go to parse a new command and send back that we're done. Otherwise, return an error.
                //      If we never filled any numbers, it's an error too.
                else if(rpCharString[dIterator] == '\n' || rpCharString[dIterator] == ';')
                {
                    if(*rpParseNumIndex)
                    {
                        if(!SetFieldValue(rpField, atof(rpParseNum), rpCommand))
                        {
                            *tParseState = PARSE_STATE_START;
                            return PARSE_RETURN_ERROR;
                        }
                        memset(rpParseNum,0, MAX_FLOAT_DIGITS);
                        *rpParseNumIndex = 0;
                        *tParseState = PARSE_STATE_PARSE_COMMAND;
                        *rdIterStart = dIterator;
                        return PARSE_RETURN_DONE;
                    }
                }
                else
                {
                    if(!AddToNum(&(rpParseNum[*rpParseNumIndex]),rpCharString[dIterator]))
                    {
                        *tParseState = PARSE_STATE_START;
                        return PARSE_RETURN_ERROR;
                    }
                    else
                    {
                        (*rpParseNumIndex)++;
                    }
                }                
                break;
        }
    }

    // On the off chance that we go through all of our current data and we're done

    *rdIterStart = 0;
    if(!bDone)
    {
        return PARSE_RETURN_READ_MORE;
    }
    else
    {
        return PARSE_RETURN_DONE;
    }

}
