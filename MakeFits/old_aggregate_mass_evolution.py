#!/usr/bin/env python3.6
import sys
import os
import math



def main(argv):
   
    output_file = "mass_evolution.h"
    input_dir = "/lustre/beloviq2/ampToolsFits/3PiBinnedFit/"
    input_file = []
    bin_center = []
    
    mass_3pi_low  = [0.750,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825]

    mass_3pi_high = [0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825,1.850]

    

    for m3pi_low, m3pi_high in zip(mass_3pi_low, mass_3pi_high):        
        filename = "isops_fitpars_tbin0_m3pi_%.3f_%.3f.txt" % (m3pi_low,m3pi_high)

        input_file.append(input_dir+filename)
        bin_center.append(1000.*(m3pi_low + m3pi_high) / 2.0)

    bin_centers_str = ", ".join("%.2f" % bc for bc in bin_center)
    num_bins = len(bin_center)
    
    
    #Read content of all of the bins into a single dictionary called 'results'    
    results = {}
    
    for current_file in input_file:
        instream = open(current_file,"r")
        first_line = instream.readline().strip()

        if "TOTAL EVENTS" in first_line:
            yield_str = first_line.split("=")[1].split("+-")[0]
        else:
            print("Error parsing TOTAL EVENTS line:", first_line)
        
        for line in instream:            
            if "FIT FRACTION (coherent sum)" in line:
                line = line.strip()

                try:
                    left, right = line.split("=")
                    name = left.split("coherent sum)")[1].strip()
                    value_str, error_str = right.split("+-")

                    name = name.replace(" ", "_")
                    value = float(yield_str)*float(value_str)
                    error = float(yield_str)*float(error_str)

                    if name not in results:
                        results[name] = {"value": [], "error": []}
                    
                    if value not in results[name]["value"]:    
                        results[name]["value"].append(value)
                        results[name]["error"].append(error)                    

                except:
                    print("Skipping line (parse error): %s" % (line))
                    
                    
        instream.close()

        
    #Convert dictionary into arrays        
    entries = []
    
    for name, data in results.items():
        values = ", ".join("%.5e" % v for v in data["value"])
        errors = ", ".join("%.5e" % e for e in data["error"])
        entries.append('{"%s", {%s}, {%s}, {%s}}' % (name, bin_centers_str, values, errors))
        
        
    # Write output as one structure with arrays in C++
    outstream = open(output_file, "w")
    outstream.write("const float BINWIDTH = %.3f;\n" % (bin_center[2]-bin_center[1]))
    outstream.write("const unsigned int NBINS = %d;\n\n" % (num_bins))
    outstream.write("struct JPLMeps {\n")
    outstream.write("  std::string name;\n")
    outstream.write("  float bin_center[NBINS];\n")
    outstream.write("  float value[NBINS];\n")
    outstream.write("  float error[NBINS];\n")
    outstream.write("};\n\n\n\n")
    outstream.write("JPLMeps terms[] = {\n")
    outstream.write(",\n".join(entries))        
    outstream.write("};\n")

    outstream.close()
    print("Saved %d waves for %d bins to %s" % (len(results), num_bins, output_file))

                    

if __name__ == "__main__":
    main(sys.argv)    
