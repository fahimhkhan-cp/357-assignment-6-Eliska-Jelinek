#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    // name, state, education, ethnicities, income, population
    // not included: housing, misc, employment
    char* county_name[128]; //TODO: make it a list [100]
    char* state[4];
    float education_bach;
    float education_hs;
    float ethnicities[7]; // List of ethnicity floats, there's 8 of them
    // float ethn_native;
    // float ethn_asian;
    // float ethn_black;
    // float ethn_hisp;
    // float ethn_islander;
    // float ethn_mixed;
    // float ethn_white;
    // float ethn_white_not_hisp;
    int median_household_income;
    int per_capita_income;
    float persons_below_poverty_level;
    int population;
} CountyInfo;

// Define global variables
CountyInfo county_data[4000];
int county_count = 0;

int parse_line2(char *line, CountyInfo *data) {
    char *county_name = strtok(line, ","); // County name
    if (!county_name) {return 1;} // No data, malformed line
    // remove quotes
    strncpy(data->county_name, county_name, sizeof(data->county_name) - 1); //strncpy because fixed length

    char *state = strtok(NULL, ",");
    if (!state) {return 1;} // No data, malformed line
    // remove quotes
    strncpy(data->state, state, sizeof(data->state) - 1); //strncpy because fixed length

    // skip age data
    for (int i = 0; i < 3; i ++) {
        strtok(NULL, ",");
    }



}

// Used ChatGPT to learn about enums
// variables match the structure variables
enum FieldIndex {
    county_name,
    state,
    education_bach,
    education_hs,
    ethn_native,
    ethn_asian,
    ethn_black,
    ethn_hisp,
    ethn_islander,
    ethn_mixed,
    ethn_white,
    ethn_white_not_hisp,
    median_household_income,
    per_capita_income,
    persons_below_poverty_level,
    population
};

const char *valid_fields[] = {
    "County",
    "State",
    "Education.Bachelor's Degree or Higher",
    "Education.High School or Higher",
    "Ethnicities.American Indian and Alaska Native Alone",
    "Ethnicities.Asian Alone",
    "Ethnicities.Black Alone",
    "Ethnicities.Hispanic or Latino",
    "Ethnicities.Native Hawaiian and Other Pacific Islander Alone",
    "Ethnicities.Two or More Races",
    "Ethnicities.White Alone",
    "Ethnicities.White Alone not Hispanic or Latino",
    "Income.Median Household Income",
    "Income.Per Capita Income",
    "Income.Persons Below Poverty Level",
    "Population.2014 Population"
};

const char *header[] = {"County","State","Age.Percent 65 and Older","Age.Percent Under 18 Years","Age.Percent Under 5 Years","Education.Bachelor's Degree or Higher","Education.High School or Higher","Employment.Nonemployer Establishments","Employment.Private Non-farm Employment","Employment.Private Non-farm Employment Percent Change","Employment.Private Non-farm Establishments","Ethnicities.American Indian and Alaska Native Alone","Ethnicities.Asian Alone","Ethnicities.Black Alone","Ethnicities.Hispanic or Latino","Ethnicities.Native Hawaiian and Other Pacific Islander Alone","Ethnicities.Two or More Races","Ethnicities.White Alone","Ethnicities.White Alone not Hispanic or Latino","Housing.Homeownership Rate","Housing.Households","Housing.Housing Units","Housing.Median Value of Owner-Occupied Units","Housing.Persons per Household","Housing.Units in Multi-Unit Structures","Income.Median Household Income","Income.Per Capita Income","Income.Persons Below Poverty Level","Miscellaneous.Building Permits","Miscellaneous.Foreign Born","Miscellaneous.Land Area","Miscellaneous.Language Other than English at Home","Miscellaneous.Living in Same House +1 Years","Miscellaneous.Manufacturers Shipments","Miscellaneous.Mean Travel Time to Work","Miscellaneous.Percent Female","Miscellaneous.Veterans","Population.2010 Population","Population.2014 Population","Population.Population Percent Change","Population.Population per Square Mile","Sales.Accommodation and Food Services Sales","Sales.Merchant Wholesaler Sales","Sales.Retail Sales","Sales.Retail Sales per Capita","Employment.Firms.American Indian-Owned","Employment.Firms.Asian-Owned","Employment.Firms.Black-Owned","Employment.Firms.Hispanic-Owned","Employment.Firms.Native Hawaiian and Other Pacific Islander-Owned","Employment.Firms.Total","Employment.Firms.Women-Owned"};

int parse_line1(char *line, CountyInfo *data) {
    
    char *county_name = strtok(line, ","); // County name
    if (!county_name) {return 1;} // No data, malformed line
    strncpy(data->county_name, county_name, sizeof(data->county_name) - 1); //strncpy because fixed length
    char *data_bit;
    int valid_data_index = 1;
    int header_index = 1;
    while (data_bit = strtok(NULL, ",")) {
        if (valid_data_index == header_index) {
            // TODO: remove quotes
            // FieldIndex[valid_data_index];
            // (data->FieldIndex[valid_data_index], data_bit, sizeof(data->FieldIndex[valid_data_index]) - 1);
        }
        valid_data_index++;
        header_index++;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// PASTED FROM GPT //////


// Function to clean a field by removing surrounding double quotes
void clean_field(char *field) {
    char *src = field, *dest = field;
    while (*src) {
        if (*src != '"') {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

#define MAX_FIELDS 16

// Parse line from CSV
int parse_line(char *line, CountyInfo *data) {
    char *fields[MAX_FIELDS];
    char *token;
    int field_index = 0;

    token = strtok(line, ",");
    while (token && field_index < MAX_FIELDS) {
        clean_field(token);
        fields[field_index++] = token;
        token = strtok(NULL, ",");
    }

    if (field_index < 15) return -1;

    strncpy(data->county_name, fields[0], sizeof(data->county_name) - 1);
    strncpy(data->state, fields[1], sizeof(data->state) - 1);
    data->education_bach = atof(fields[2]);
    data->education_hs = atof(fields[3]);
    for (int i = 4; i < 11; i++) {
        data->ethnicities[i - 4] = atof(fields[i]);
    }
    data->median_household_income = atoi(fields[11]);
    data->per_capita_income = atoi(fields[12]);
    data->persons_below_poverty_level = atof(fields[13]);
    data->population = atoi(fields[14]);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void display() {
    for (int c = 1; c < county_count; c++) {
        CountyInfo county = county_data[c];
        printf("%s, %s:\n", county.county_name, county.state);
        printf("\tBachelor or Higher: %.2f%%\n", county.education_bach);
        printf("\tHigh School or Higher: %.2f%%\n", county.education_hs);
        printf("\tEthnicities:\n");
        printf("\t\tAmerican Indian and Alaska Native: %.2f%%\n", county.ethnicities[0]);
        printf("\t\tAsian: %.2f%%\n", county.ethnicities[1]);
        printf("\t\tBlack: %.2f%%\n", county.ethnicities[2]);
        printf("\t\tHispanic or Latino: %.2f%%\n", county.ethnicities[3]);
        printf("\t\tPacific Islander: %.2f%%\n", county.ethnicities[4]);
        printf("\t\tMixed: %.2f%%\n", county.ethnicities[5]);
        printf("\t\tWhite: %.2f%%\n", county.ethnicities[6]);
        printf("\t\tWhite, not Hispanic or Latino: %.2f%%\n", county.ethnicities[7]);
        printf("\tMedian Household Income: $%d\n", county.median_household_income);
        printf("\tPer Capita Income: $%d\n", county.per_capita_income);
        printf("\tersons Below Poverty Level: %.2f%%\n", county.persons_below_poverty_level);
        printf("\tPopulation: %d\n", county.population);
    } 
}

void filter_state(char *state) {
    int remaining = 0;
    for (int c = 0; c < county_count; c++) {
        if (strcmp(county_data[c].state, state) == 0) {
            remaining++;
        }
    }
    printf("Filter by State: state = %s, %d entries\n", state, remaining);

}

void filter_number(char *field, char *comparison, float number) {

}

void population_total() {
    
}

void population_field() {

}

void percent() {

} 






int main(int argc, char *argv[]) {
    // argv = [program name, demographics file, operations file] 
    if (argc != 3) {
        perror("Invalid arguments (not enough or too many). Exiting program.");
        return 1;
    }

    // Try to open demographics file for reading
    FILE *demographics_file = fopen(argv[1], "r");
    if (!demographics_file) {
        perror("Error opening demographics file. Exiting program.");
        return 1;
    }

    char line[2048];
    int line_num = 0;
    int valid_entry_count = 0;

    // Get first line (skip), and verify file has contents
    if (fgets(line, sizeof(line), demographics_file) == NULL) {
        perror("No demographic data found (file is empty). Exiting program.");
        fclose(demographics_file);
        return 1;
    }


    // CountyInfo county_data[4000]; // Will not have more counties than that

    while (fgets(line, sizeof(line), demographics_file)) {
        line_num++;
        CountyInfo data;
        if (parse_line(line, &data) != 0) {
            fprintf(stderr, "Malformed data [line %d]\n", line_num);
        } else {
            valid_entry_count++;
            county_data[valid_entry_count] = data;
        }
    }
    county_count = valid_entry_count;

    // Done reading data, close file
    fclose(demographics_file);
    printf("%d entries loaded.\n", county_count);




    // Execute operations
    // Open operations file
    FILE *operations_file = fopen(argv[2],"r");
    if (!operations_file) {
        perror("Error opening operations file. Exiting program.");
        return 1;
    }

    char op_line[1024]; // TODO
    int op_line_num = 0;

    while (fgets(op_line, sizeof(op_line), operations_file)) {
        op_line_num++;
        // Initial vars to save operation arguments
        char operation[sizeof(op_line)];
        char field[sizeof(op_line)];
        char comparison[sizeof(op_line)];
        float number;

        // Identify operation, map to argument variables
        if (strcmp(op_line, "display\n") == 0) {
            display();
        } else if (sscanf(op_line, "filter-state:%2s", field) == 1) { // abbrv are not more than 2 letters
            filter_state(field);
        } else if (strncmp(op_line, "filter:", strlen("filter:")) == 0){
            char *filter = strtok(op_line, ":");
            *field = strdup(strtok(NULL, ":"));
            *comparison = strdup(strtok(NULL, ":"));
            //char *number_str = strdup(strtok(NULL, ":"));
            //number_str[strlen(number_str)-1] = '\0';
            //number = atof(number_str);
            number = atof(strdup(strtok(NULL, ":")));
            filter_number(field, comparison, number);
        } else if (strcmp(op_line, "population-total\n") == 0) {
            population_total();
        } else if (sscanf(op_line, "population:%s", field) == 1) {
            printf("field: %s.", field);
            population_field(field);
        } else {
            fprintf(stderr, "Error: Malformed operation on line %d\n", op_line_num);
        }
    }

    // Close operations file
    fclose(operations_file);


    // Return success
    return 0;
}