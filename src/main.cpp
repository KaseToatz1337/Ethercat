#include <ethercat.h>
#include <iostream>

int main()
{
	if (!ec_init("eth0"))
	{
		return 1;
	}
	if (ec_config_init(FALSE) < 1)
	{
		return 1;
	}
	char IOmap[4096];
	ec_config_map(&IOmap);
	ec_configdc();
	ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);
	uint32_t oloop = ec_slave[0].Obytes;
	if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
	if (oloop > 8) oloop = 8;
	uint32_t iloop = ec_slave[0].Ibytes;
	if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
	if (iloop > 8) iloop = 8;
	int expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
	ec_slave[0].state = EC_STATE_OPERATIONAL;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	ec_writestate(0);
	for (int i = 0; i < 200; i++)
	{
		if (ec_slave[0].state == EC_STATE_OPERATIONAL)
		{
			break;
		}
	}
	if (ec_slave[0].state != EC_STATE_OPERATIONAL)
	{
		return 1;
	}
	std::cout << "SLAVE IS SLAVING!!!" << std::endl;
	return 0;
}