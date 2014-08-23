/*
 * =========================================================================================
 * name        : maman_14/systemData.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#include "systemData.h"

/* 
 * Array of maskes requested by width 
 */
unsigned char maskWidthArray[] = {0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};

/* 
 * @descriptoin after allocating memory, the data need to be clear from garbage 
 */
void registerDataLine(data_line *dl)
{
    dl->data = 0;
}

/*
 * @decription returning data_line containing the char in the argument 
 */
data_line char2data(char ch)
{
    data_line tmp;
    registerDataLine(&tmp);
    tmp.data = ch;
    return tmp;
}

/* 
 * @description returning data_line containing the (boolean) instruction line in the argument 
 */
data_line bline2data(instructionLineObject bl)
{
    data_line tmp;
    registerDataLine(&tmp);

    tmp.data |= (maskWidthArray[COMB_WIDTH]   << COMB_OFFSET)   & (bl.comb << COMB_OFFSET);
    tmp.data |= (maskWidthArray[DREG_WIDTH]   << DREG_OFFSET)   & (bl.destReg << DREG_OFFSET);
    tmp.data |= (maskWidthArray[DADDR_WIDTH]  << DADDR_OFFSET)  & (bl.destAddr << DADDR_OFFSET);
    tmp.data |= (maskWidthArray[SREG_WIDTH]   << SREG_OFFSET)   & (bl.srcReg << SREG_OFFSET);
    tmp.data |= (maskWidthArray[SADDR_WIDTH]  << SADDR_OFFSET)  & (bl.srcAddr << SADDR_OFFSET);
    tmp.data |= (maskWidthArray[OPCODE_WIDTH] << OPCODE_OFFSET) & (bl.opcode << OPCODE_OFFSET);
    tmp.data |= (maskWidthArray[TYPE_WIDTH]   << TYPE_OFFSET)   & (bl.type << TYPE_OFFSET);
    tmp.data |= (maskWidthArray[DBL_WIDTH]    << DBL_OFFSET)    & (bl.dbl << DBL_OFFSET);
    tmp.data |= (maskWidthArray[RSVD_WIDTH]   << RSVD_OFFSET)   & (bl.rsvd << RSVD_OFFSET);

    return tmp;
}

/* 
 * @description returning data_line containing the int in the argument with the sign bit 
 */
data_line num2data(int num)
{
    unsigned int bit_mask_19 = 0x7FFFF;
    data_line tmp;
    registerDataLine(&tmp);

    if (num >= 0)
    {
        tmp.data = num & bit_mask_19;
    }
    else
    {
        tmp.data = (num * -1) & bit_mask_19;
        tmp.data = ~(tmp.data);
        (tmp.data)++;
    }

    return tmp;
}

/* 
 * @description clearing the values of an instruction line 
 */
void registerInstructionLine(instructionLineObject *il)
{
    il->comb = 0;
    il->destReg = 0;
    il->destAddr = 0;
    il->srcReg = 0;
    il->srcAddr = 0;
    il->opcode = 0;
    il->type = 0;
    il->dbl = 0;
    il->rsvd = 0; 
}
