#pragma once

#include "TspSwarm.h"

namespace WMH {
	/** Test klasy Graph */
	void graph_test();
	/** Testuje wynika dla stalych grafow */
	void same_graphs_test(	float C1 = PSO::TspSwarm::DEF_C1, 
							float C2 = PSO::TspSwarm::DEF_C2, 
							float OMEGA = PSO::TspSwarm::DEF_OMEGA, 
							int REHOPE = PSO::TspSwarm::DEF_REHOPE);
	/** Wyszukuje najlepsze parametry */
	void params_test();
	/** Zapisuje losowy graf do pliku */
	void generate_and_save(const char* filename, int vertices);
}
