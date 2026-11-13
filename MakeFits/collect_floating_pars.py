#!/usr/bin/env python3.6
import sys
import os


def main(argv):

    input_dir = "/lustre/beloviq2/ampToolsFits/3PiBinnedFitFloatIsoBWs/"
    tbin_number = 0
    bin_center = []

    
    mass_3pi_low = [0.750,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,
1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825]

    mass_3pi_high=[0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825,1.850]

    # Parameter names and their corresponding line numbers.
    parameters = {"deltaMass":682, "deltaWidth":683, "rhoMass":684, "rhoWidth":685, "f2Mass":686, "f2Width":687}
    results = {name: [] for name in parameters}

    # Loop over the mass bins.
    for m3pi_low, m3pi_high in zip(mass_3pi_low, mass_3pi_high):

        bin_center.append(1000.*(m3pi_low + m3pi_high) / 2.0)

        current_folder = "tbin%d_m3pi_%.3f_%.3f" % (tbin_number, m3pi_low, m3pi_high)
        current_folder = os.path.join(input_dir, current_folder)

        if not os.path.isdir(current_folder):
            raise RuntimeError("Directory does not exist: %s" % current_folder)

        current_file = os.path.join(current_folder,next(f for f in os.listdir(current_folder) if f.endswith(".fit") and f.count("_") == 5))

        # Read the file once.
        with open(current_file, "r") as instream:
            lines = instream.readlines()

        # Extract the six parameters from their respective lines.
        for name, line_number in parameters.items():

            if len(lines) < line_number:
                raise RuntimeError("File %s has fewer than %d lines"     % (current_file, line_number))

            fields = lines[line_number - 1].split()

            if len(fields) < 2 or fields[0] != name:
                raise RuntimeError("Unexpected content at line %d in %s: %s"   % (line_number, current_file,lines[line_number - 1].strip()))

            results[name].append(float(fields[1]))

    # Print the C++ initializer.
    bin_centers_str = ", ".join("%.3f" % x for x in bin_center)

    print("\nfloatPar floatingBW[] = {")

    parameter_entries = []

    for name in parameters:

        values_str = ", ".join("%.3f" % value for value in results[name])
        entry = ('{"%s",{%s},{%s}}' % (name, bin_centers_str, values_str))
        parameter_entries.append(entry)

    print(",\n".join(parameter_entries))
    print("};")

    print("\nProcessed %d mass bins and %d parameters."   % (len(bin_center), len(parameters)))


if __name__ == "__main__":
    main(sys.argv[1:])
