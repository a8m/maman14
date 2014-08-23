/*
 * =========================================================================================
 * name        : maman_14/data.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#include "data.h"

/* 
 * Array of maskes requested by width 
 */
unsigned char mask_by_width[] = {0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};

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
data_line bline2data(instruction_line bl)
{
    data_line tmp;
    registerDataLine(&tmp);

    tmp.data |= (mask_by_width[COMB_WIDTH]   << COMB_OFFSET)   & (bl.comb << COMB_OFFSET);
    tmp.data |= (mask_by_width[DREG_WIDTH]   << DREG_OFFSET)   & (bl.dest_reg << DREG_OFFSET);
    tmp.data |= (mask_by_width[DADDR_WIDTH]  << DADDR_OFFSET)  & (bl.dest_addr << DADDR_OFFSET);
    tmp.data |= (mask_by_width[SREG_WIDTH]   << SREG_OFFSET)   & (bl.src_reg << SREG_OFFSET);
    tmp.data |= (mask_by_width[SADDR_WIDTH]  << SADDR_OFFSET)  & (bl.src_addr << SADDR_OFFSET);
    tmp.data |= (mask_by_width[OPCODE_WIDTH] << OPCODE_OFFSET) & (bl.opcode << OPCODE_OFFSET);
    tmp.data |= (mask_by_width[TYPE_WIDTH]   << TYPE_OFFSET)   & (bl.type << TYPE_OFFSET);
    tmp.data |= (mask_by_width[DBL_WIDTH]    << DBL_OFFSET)    & (bl.dbl << DBL_OFFSET);
    tmp.data |= (mask_by_width[RSVD_WIDTH]   << RSVD_OFFSET)   & (bl.rsvd << RSVD_OFFSET);

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
void registerInstructionLine(instruction_line *il)
{
    il->comb = 0;
    il->dest_reg = 0;
    il->dest_addr = 0;
    il->src_reg = 0;
    il->src_addr = 0;
    il->opcode = 0;
    il->type = 0;
    il->dbl = 0;
    il->rsvd = 0; /***?***/
}

