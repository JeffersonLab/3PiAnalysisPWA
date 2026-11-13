#!/usr/bin/env python3.6
import sys
import os
import math



def main(argv):
   
    input_dir = "/lustre/beloviq2/ampToolsFits/3PiFitBinned25MeV/FloatIsoBWs/"
    line_number = 238
    tbin_number = 0

    mass_3pi_low  = [0.750,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825]

    mass_3pi_high = [0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825,1.850]

    
    #Read content of all of the bins into a single dictionary called 'results'    
    output_file = "minimized_likelihoods_tbin%d.h" %(tbin_number)
    bin_center = []
    results = {}
    
    #Loop over folders with m3pi bins
    for m3pi_low, m3pi_high in zip(mass_3pi_low, mass_3pi_high):        

        bin_center.append(1000.*(m3pi_low + m3pi_high) / 2.0)
        current_folder = "tbin%d_m3pi_%.3f_%.3f" % (tbin_number,m3pi_low, m3pi_high)
        current_folder = os.path.join(input_dir, current_folder)
                
        if current_folder not in results:
            results[current_folder] = {"Minimum": []}
        
        #loop over files with fit results
        for filename in sorted(os.listdir(current_folder)):
            if not filename.endswith(".fit"): continue

            current_file = os.path.join(current_folder,filename)
            instream = open(current_file, "r")
               

            # Read the requested line 
            fcn_line = None
            for i, line in enumerate(instream, start=1):
                if i == line_number:
                    fcn_line = line.strip()
                    break

            if fcn_line is None:
                print("No line number %d in %s" % (line_number, current_file))
                continue
                
            if "bestMinimum" in fcn_line: 
                fcn = float(fcn_line.split()[1])
                results[current_folder]["Minimum"].append(fcn)
            else:
                print("Error parsing the line with bestMinimum")

            instream.close()

        
    bin_centers = ", ".join("%.2f" % bc for bc in bin_center)
    bin_centers_str = "{" + bin_centers + "}"
    num_bins = len(bin_center)


    #Convert dictionary into arrays        
    entries = []
    
    for folder, data in results.items():
        minima = ", ".join("%.5e" % v for v in data["Minimum"])      
        entries.append("{" + minima + "}")
    num_fits = len(data["Minimum"])    

        
        
    # Write output as one structure with arrays in C++
    outstream = open(output_file, "w")
    outstream.write("const float BINWIDTH = %.3f;\n" % (bin_center[2]-bin_center[1]))
    outstream.write("const unsigned int NBINS = %d;\n" % (num_bins))
    outstream.write("const unsigned int NFITS = %d;\n\n" % (num_fits))
    outstream.write("struct Fit {\n")
    outstream.write("  std::string trange;\n")
    outstream.write("  float bin_center[NBINS];\n")
    outstream.write("  float minimum[NBINS][NFITS];\n")
    outstream.write("};\n\n\n\n")
    outstream.write("Fit Delta3Pi[] = {\n")
    outstream.write("\"tbin%d\"," % (tbin_number))
    outstream.write(bin_centers_str)
    outstream.write(",\n{")
    outstream.write(",\n".join(entries))        
    outstream.write("}\n};\n")

    outstream.close()
    print("Saved minima for %d bins (%d fits each) to %s" % (num_bins, num_fits, output_file))

    
        

if __name__ == "__main__":
    main(sys.argv)    
