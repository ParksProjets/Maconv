/*

Maconv: Macintosh format converter.

Copyright (C) 2019, Guillaume Gonnet

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "commands.h"

#include <CLI11.hpp>
#include <cstdarg>

using namespace maconv;


// Is the verbose mode enabled?
bool maconv::verbose;



// Log debug message (if verbose mode is enabled).
extern "C" void LogDebug(const char *fmt, ...)
{
    if (!verbose) return;

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    va_end(args);
    printf("\n");
}


// Stop the program on an error.
extern "C" void StopOnError(const char *fmt, ...)
{
    va_list args;
    fprintf(stderr, "\033[1m\033[31mERROR: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\033[0m\n");
    exit(1);
}



// Entry point of the application.
int main(int argc, char **argv)
{
    // Create the base command.
    CLI::App app {"Maconv: Macintosh format converter"};
    app.require_subcommand();

    app.add_flag("-v,--verbose", verbose, "Enable verbose mode");


    // Convert "c" sub-command.
    auto c_app = app.add_subcommand("c", "Convert a file from a format to another");

    std::string c_input;
    c_app->add_option("input", c_input, "Input file to convert")
        ->required()
        ->type_name("<filename>");

    std::string c_output;
    c_app->add_option("output", c_output, "Output converted file (autodetected by default)")
        ->type_name("<filename>");

    std::string c_format;
    c_app->add_option("-f,--format", c_format, "Target format (autodetected or rsrc by default)")
        ->type_name("<format>");


    // Extract "e" sub-command.
    auto e_app = app.add_subcommand("e", "Extract a Stuffit archive or a disk file");

    std::string e_input;
    e_app->add_option("input", e_input, "Input file to extract")
        ->required()
        ->type_name("<filename>");

    std::string e_output;
    e_app->add_option("output", e_output, "Output folder (current folder by default)")
        ->default_val(".")
        ->type_name("<folder name>");

    std::string e_format;
    e_app->add_option("-f,--format", e_format, "Res. format (rsrc by default)")
        ->default_val("rsrc")
        ->type_name("<format>");


    // Disk creation "d" sub-command.
    auto d_app = app.add_subcommand("d", "Create an HFS disk file");

    std::string d_folder;
    d_app->add_option("folder", d_folder, "Root of the disk")
        ->required()
        ->type_name("<folder name>");

    std::string d_output;
    d_app->add_option("output", d_output, "Output disk file")
        ->type_name("<filename>");

    std::string d_name;
    d_app->add_option("-n,--name", d_name, "Volume name (Input folder name by default)")
        ->type_name("<name>");


    // Parse the CLI.
    CLI11_PARSE(app, argc, argv);

    // Select the right command to execute.
    if (*c_app)
        RunConvertCommand(c_input, c_output, c_format);
    else if (*e_app)
        RunExtractCommand(e_input, e_output, e_format);
    else if (*d_app)
        RunDiskCommand(d_folder, d_output, d_name);
}
