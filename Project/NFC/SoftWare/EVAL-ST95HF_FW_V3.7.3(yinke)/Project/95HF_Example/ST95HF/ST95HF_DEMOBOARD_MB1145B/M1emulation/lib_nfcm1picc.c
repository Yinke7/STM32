#include "lib_nfcm1picc.h"


int8_t PICCNFCT2_ReplyCommand( uc8 *pData )
{
	uint8_t	InsCode = pData[PICC_DATA_OFFSET];
	
	switch (InsCode)
	{
		case PICCNFCT2_SECTOR_SELECT :
			PICCNFCT2_SelectSector ( pData );
		break;
		
        case M1_AUTHENTICATION_A:
            /*Code A*/
            break;
        case M1_AUTHENTICATION_B:
            /*Code B*/
            break;
        
		case M1_READ:
			PICCNFCM1_Read(pData);
		break;
		
		case M1_WRITE:
			PICCNFCM1_Write(pData);
		break;
        
		default:
			if (flag_sector_select == 1) /* Catches the SELECT_SECTOR packet 2 */
				PICCNFCT2_SelectSector ( pData );
			else
				return PICCNFCT2_ERRORCODE_COMMANDUNKNOWN;
	}
	
	return PICCNFCT2_SUCCESSCODE; 
}

