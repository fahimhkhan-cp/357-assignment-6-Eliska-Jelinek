#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    // name, state, education, ethnicities, income, population
    // not included: housing, misc, employment
    char county_name[100]; 
    char state[4];
    float education_bach;
    float education_hs;
    float ethnicities[8]; // List of ethnicity floats, there's 8 of them
    int median_household_income;
    int per_capita_income;
    float persons_below_poverty_level;
    int population;
} CountyInfo;

// Define global variables
CountyInfo county_data[3145];
int county_count = 0;

int parse_line(char *line, CountyInfo *county) {
    // Tokenize the line
    char *token = strtok(line, ",");
    int token_index = 0;

    while (token != NULL) {
        // Remove surrounding quotes, if any
        if (token[0] == '"' && token[strlen(token) - 1] == '"') {
            token[strlen(token) - 1] = '\0';
            token++; // Skip the opening quote
        }

        if (token_index == 0) { // County Name
            strncpy(county->county_name, token, sizeof(county->county_name) - 1);
            county->county_name[sizeof(county->county_name) - 1] = '\0'; // Ensure null-termination
        } else if (token_index == 1) { // State
            strncpy(county->state, token, sizeof(county->state) - 1);
            county->state[sizeof(county->state) - 1] = '\0'; // Ensure null-termination
        } else if (token_index == 5) { // Education - Bachelor's Degree or Higher
            county->education_bach = atof(token);
        } else if (token_index == 6) { // Education - High School or Higher
            county->education_hs = atof(token);
        } else if (token_index == 11) { // Ethnicities - Native American
            county->ethnicities[0] = atof(token);
        } else if (token_index == 12) { // Ethnicities - Asian
            county->ethnicities[1] = atof(token);
        } else if (token_index == 13) { // Ethnicities - Black
            county->ethnicities[2] = atof(token);
        } else if (token_index == 14) { // Ethnicities - Hispanic
            county->ethnicities[3] = atof(token);
        } else if (token_index == 15) { // Ethnicities - Native Hawaiian
            county->ethnicities[4] = atof(token);
        } else if (token_index == 16) { // Ethnicities - Two or More Races
            county->ethnicities[5] = atof(token);
        } else if (token_index == 17) { // Ethnicities - White
            county->ethnicities[6] = atof(token);
        } else if (token_index == 25) { // Median Household Income
            county->median_household_income = atoi(token);
        } else if (token_index == 26) { // Per Capita Income
            county->per_capita_income = atoi(token);
        } else if (token_index == 27) { // Persons Below Poverty Level
            county->persons_below_poverty_level = atof(token);
        } else if (token_index == 38) { // Population
            county->population = atoi(token);
        }

        // Move to the next token
        token = strtok(NULL, ",");
        token_index++;
    }
    return 0;
}


void display() {
    int c = 0;
    for (c; c < county_count; c++) {
        CountyInfo county = county_data[c];
        printf("%s, %s:\n", county.county_name, county.state);
        printf("\tBachelor or Higher: %f%%\n", county.education_bach);
        printf("\tHigh School or Higher: %f%%\n", county.education_hs);
        printf("\tEthnicities:\n");
        printf("\t\tAmerican Indian and Alaska Native: %f%%\n", county.ethnicities[0]);
        printf("\t\tAsian: %f%%\n", county.ethnicities[1]);
        printf("\t\tBlack: %f%%\n", county.ethnicities[2]);
        printf("\t\tHispanic or Latino: %f%%\n", county.ethnicities[3]);
        printf("\t\tPacific Islander: %f%%\n", county.ethnicities[4]);
        printf("\t\tMixed: %f%%\n", county.ethnicities[5]);
        printf("\t\tWhite: %f%%\n", county.ethnicities[6]);
        printf("\t\tWhite, not Hispanic or Latino: %f%%\n", county.ethnicities[7]);
        printf("\tMedian Household Income: $%d\n", county.median_household_income);
        printf("\tPer Capita Income: $%d\n", county.per_capita_income);
        printf("\tPersons Below Poverty Level: %f%%\n", county.persons_below_poverty_level);
        printf("\tPopulation: %d\n", county.population);
    } 
}

void filter_state(char *state) {
    int remaining = 0;
    int c = 0;
    for (c; c < county_count; c++) {
        if (strcmp(county_data[c].state, state) == 0) {
            county_data[remaining++] = county_data[c];
        }
    }
    county_count = remaining;
    printf("Filter: state == %s (%d entries)\n", state, county_count);

}



void filter_number(char *field, char *comparison, float number) {
    int remaining = 0;
    int c = 0;
    for (c; c < county_count; c++) {
        float field_value = 0.0;
        // identify field, get value of that field
        if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
            field_value = county_data[c].education_bach;
        } else if (strcmp(field, "Education.High School or Higher") == 0) {
            field_value = county_data[c].education_hs;
        } else if (strcmp(field, "Income.Median Household Income") == 0) {
            field_value = county_data[c].median_household_income;
        } else if (strcmp(field, "Income.Per Capita Income") == 0) {
            field_value = county_data[c].per_capita_income;
        } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
            field_value = county_data[c].persons_below_poverty_level;
        } else if (strcmp(field, "Population.2014 Population") == 0) {
            field_value = county_data[c].population;
        } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
            field_value = county_data[c].ethnicities[0];
        } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
            field_value = county_data[c].ethnicities[1];
        } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
            field_value = county_data[c].ethnicities[2];
        } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
            field_value = county_data[c].ethnicities[3];
        } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
            field_value = county_data[c].ethnicities[4];
        } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
            field_value = county_data[c].ethnicities[5];
        } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
            field_value = county_data[c].ethnicities[6];
        } else if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) {
            field_value = county_data[c].ethnicities[7];
        }

        // Compare as requested
        if ((strcmp(comparison, "ge") == 0 && field_value >= number) 
            ||
            (strcmp(comparison, "le") == 0 && field_value <= number)){
            county_data[remaining++] = county_data[c];
        }
    }
    county_count = remaining;
    printf("Filter: %s %s %f (%d entries)\n", field, comparison, number, county_count);

}

void population_total() {
    int total = 0;
    int c = 0;
    for (c; c < county_count; c++) {
        total += county_data[c].population;
    }
    printf("2014 population: %d\n", total);
}

void population_field(char *field) {
    int total = 0;
    int c = 0;
    for (c; c < county_count; c++) {
        float percentage = 0.0;
        // identify field, get value of that field
        if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
            percentage = county_data[c].education_bach;
        } else if (strcmp(field, "Education.High School or Higher") == 0) {
            percentage = county_data[c].education_hs;
        } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
            percentage = county_data[c].persons_below_poverty_level;
        } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
            percentage = county_data[c].ethnicities[0];
        } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
            percentage = county_data[c].ethnicities[1];
        } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
            percentage = county_data[c].ethnicities[2];
        } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
            percentage = county_data[c].ethnicities[3];
        } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
            percentage = county_data[c].ethnicities[4];
        } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
            percentage = county_data[c].ethnicities[5];
        } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
            percentage = county_data[c].ethnicities[6];
        } else if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) {
            percentage = county_data[c].ethnicities[7];
        }
        total += (int)(county_data[c].population * (percentage / 100));
    }
    printf("2014 %s population: %d\n", field, total);
}

void percent(char *field) {
    float total_pop = 0.0;
    float total_sub_pop = 0.0; 

    int c = 0;
    for (c; c < county_count; c++) {
        total_pop += county_data[c].population;

        // Check the field and sum up the corresponding sub-population
        if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
            total_sub_pop += (county_data[c].education_bach / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Education.High School or Higher") == 0) {
            total_sub_pop += (county_data[c].education_hs / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
            total_sub_pop += (county_data[c].ethnicities[0] / 100.0) * county_data[c].population; // Adjust index based on the correct ethnicity
        } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
            total_sub_pop += (county_data[c].ethnicities[1] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
            total_sub_pop += (county_data[c].ethnicities[2] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
            total_sub_pop += (county_data[c].ethnicities[3] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
            total_sub_pop += (county_data[c].ethnicities[4] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
            total_sub_pop += (county_data[c].ethnicities[5] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
            total_sub_pop += (county_data[c].ethnicities[6] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Ethnicities.White Alone, not Hispanic or Latino") == 0) {
            total_sub_pop += (county_data[c].ethnicities[7] / 100.0) * county_data[c].population;
        } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
            total_sub_pop += (county_data[c].persons_below_poverty_level / 100.0) * county_data[c].population;
        }    
    }

    // Calculate percentage
    if (total_pop > 0) { // Avoid divide by 0 error
        float percentage = (total_sub_pop / total_pop) * 100;
        printf("2014 %s percentage: %f%%\n", field, percentage);
    } else {
        printf("2014 %s percentage: 0%%\n", field);
    }

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

    char line[1900];
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
        CountyInfo county;
        if (parse_line(line, &county) != 0) {
            fprintf(stderr, "Malformed data [line %d]\n", line_num);
        } else {
            valid_entry_count++;
            county_data[valid_entry_count] = county;
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

    char op_line[128]; 
    int op_line_num = 0;

    while (fgets(op_line, sizeof(op_line), operations_file)) {
        op_line_num++;
        // Initial vars to save operation arguments
        char field[sizeof(op_line)];

        // Identify operation, map to argument variables
        if (strcmp(op_line, "display\n") == 0) {
            display();
        } else if (sscanf(op_line, "filter-state:%2s", field) == 1) { // abbrv are not more than 2 letters
            filter_state(field);
        } else if (strncmp(op_line, "filter:", strlen("filter:")) == 0){
            char *filter = strtok(op_line, ":");
            char *f = strtok(NULL, ":");
            char *comparison = strtok(NULL, ":");
            char *number_str = strdup(strtok(NULL, ":"));
            float number = atof(number_str);
            free(number_str);
            filter_number(f, comparison, number);
            
        } else if (strcmp(op_line, "population-total\n") == 0) {
            population_total();
        } else if (strncmp(op_line, "population:", strlen("population:")) == 0) {
            char *op = strtok(op_line, ":");
            char *f = strtok(NULL, ":");
            f[strlen(f)-1] = '\0';
            population_field(f);
        } else if (strncmp(op_line, "percent:", strlen("percent:")) == 0) {
            char *op = strtok(op_line, ":");
            char *f = strtok(NULL, ":");
            f[strlen(f)-1] = '\0';
            percent(f);
        } else {
            fprintf(stderr, "Error: Malformed operation on line %d\n", op_line_num);
        }
    }

    // Close operations file
    fclose(operations_file);


    // Return success
    return 0;
}