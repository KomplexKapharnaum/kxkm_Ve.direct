void lcd_start()
{
    const int delay_set = 100;
    M5.begin();       // Init M5Core
    M5.Power.begin(); // Init Power module.

    /*
    M5.Lcd.fillScreen(WHITE); //
    delay(delay_set);
    M5.Lcd.fillScreen(RED);
    delay(delay_set);
    M5.Lcd.fillScreen(GREEN);
    delay(delay_set);
    M5.Lcd.fillScreen(BLUE);
    delay(delay_set);
        */
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("KXKM_M5 Victron > InfluxDB");
}

void lcd_print(int x, int y, int color, int size, char *text) // x,y,color,size,text
{
    M5.Lcd.setTextColor(color);
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextSize(size);
    M5.Lcd.setCursor(x, y);
    M5.Lcd.print(text);
}

void lcd_menu_print()
{

    const int text_size = 2;   // text size for line in the menu
    const int pos_line_1 = 55; // line position for menu
    const int nb_line = 6;     // number of line in the menu

    // set array of line position
    int ligne[nb_line] = {pos_line_1};
    for (int i = 1; i < nb_line; i++)
    {
        ligne[i] = pos_line_1 + i * text_size * 10;
    }

    switch (lcd_menu)
    {
    case 1:
        lcd_print(10, ligne[0], WHITE, text_size, "Battery  ");
        M5.Lcd.print(data_value[0], 2);
        M5.Lcd.print(" V");
        lcd_print(10, ligne[1], WHITE, text_size, "Battery  ");
        M5.Lcd.print(data_value[1], 2);
        M5.Lcd.print(" A");
        lcd_print(10, ligne[2], WHITE, text_size, "  PV     ");
        M5.Lcd.print(data_value[2], 2);
        M5.Lcd.print(" V");
        lcd_print(10, ligne[3], WHITE, text_size, "  PV     ");
        M5.Lcd.print(data_value[3], 2);
        M5.Lcd.print(" W");
        lcd_print(10, ligne[4], WHITE, text_size, "State   ");
        switch (int(data_value[4]))
        {
        case 0:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("  Off");
            break;
        case 1:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("  Low Power");
            break;
        case 2:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("  Fault");
            break;
        case 3:
            M5.Lcd.setTextColor(BLUE);
            M5.Lcd.print("  Bulk");
            break;
        case 4:
            M5.Lcd.setTextColor(ORANGE);
            M5.Lcd.print("Absorption");
            break;
        case 5:
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.print("  Float");
            break;
        case 6:
            M5.Lcd.setTextColor(GREENYELLOW);
            M5.Lcd.print("  Storage");
            break;
        case 9:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Inverting");
            break;
        case 11:
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.print("Power Supply");
            break;
        case 245:
            M5.Lcd.print("Starting-up");
            break;
        case 246:
            M5.Lcd.setTextColor(GREENYELLOW);
            M5.Lcd.print("Repeated Absorption");
            break;
        case 247:
            M5.Lcd.setTextColor(GREENYELLOW);
            M5.Lcd.print("Auto Equalize");
            break;
        case 248:
            M5.Lcd.setTextColor(GREENYELLOW);
            M5.Lcd.print("BatterySafe");
            break;
        case 252:
            M5.Lcd.setTextColor(YELLOW);
            M5.Lcd.print("External Control");
            break;
        default:
            M5.Lcd.print("Unknown");
            break;
        }
        lcd_print(10, ligne[5], WHITE, text_size, "Tracker Mode: ");
        switch (int(data_value[5]))
        {
        case 0:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Off");
            break;
        case 1:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("Voltage or current limited");
            break;
        case 2:
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.print("MPPT active");
            break;
        default:
            M5.Lcd.print("Unknown");
            break;
        }
        break;
    case 2:
        lcd_print(10, ligne[1], WHITE, text_size, "Load Status: ");
        if (int(data_value[8]) == 0)
        {
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("OFF");
        }
        else if (int(data_value[8]) == 1)
        {

            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.print("ON");
        }
        lcd_print(10, ligne[2], WHITE, text_size, "Load Current: ");
        M5.Lcd.print(data_value[9], 2);
        M5.Lcd.print(" A");
        lcd_print(10, ligne[3], WHITE, text_size, "Off Reason: ");
        M5.Lcd.print(data_value[6]);
        lcd_print(10, ligne[5], WHITE, text_size, "Error Code: ");
        switch (int(data_value[7]))
        {
        case 0:
            M5.Lcd.print("No error");
            break;
        case 2:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Battery voltage too high");
            break;
        case 17:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Charger temperature too high");
            break;
        case 18:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Charger over current");
            break;
        case 19:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Charger current reversed");
            break;
        case 20:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("Bulk time limit exceeded");
            break;
        case 21:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Current sensor issue (sensor bias/sensor broken)");
            break;
        case 26:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Terminals overheated");
            break;
        case 33:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Input voltage too high (solar panel)");
            break;
        case 34:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Input current too high (solar panel)");
            break;
        case 38:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Input shutdown (due to excessive battery voltage)");
            break;
        case 39:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Input shutdown (due to current flow during off mode)");
            break;
        case 65:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("Lost communication with one of devices");
            break;
        case 66:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("Synchronised charging device configuration issue");
            break;
        case 67:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("BMS connection lost");
            break;
        case 68:
            M5.Lcd.setTextColor(PURPLE);
            M5.Lcd.print("Network misconfigured");
            break;
        case 116:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Factory calibration data lost");
            break;
        case 117:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Invalid/incompatible firmware");
            break;
        case 119:
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("User settings invalid");
            break;
        default:
            M5.Lcd.print("Unknown");
            break;
        }

        break;
    case 3:
        lcd_print(10, ligne[0], WHITE, text_size, "Yeld Total: ");
        M5.Lcd.print(data_value[10], 2);
        M5.Lcd.print(" kWh");
        lcd_print(10, ligne[1], WHITE, text_size, "Yeld Today: ");
        M5.Lcd.print(data_value[11], 2);
        M5.Lcd.print(" kWh");
        lcd_print(10, ligne[2], WHITE, text_size, "Max Power Today: ");
        M5.Lcd.print(data_value[12]);
        M5.Lcd.print(" W");
        lcd_print(10, ligne[3], WHITE, text_size, "Yeld Yesterday: ");
        M5.Lcd.print(data_value[13], 2);
        M5.Lcd.print(" kWh");
        lcd_print(10, ligne[4], WHITE, text_size, "Max Power Yesterday: ");
        M5.Lcd.print(data_value[14]);
        M5.Lcd.print(" W");
        break;
    default:
        break;
    }
}

void lcd_clear()
{
    // Clear the screen
    M5.Lcd.clear(BLACK);

    // Print the title
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("KXKM_M5 Victron > InfluxDB");

    // Print status of InfluxDB connection
    if (data_value[16] == 1)
    {
        lcd_print(0, 200, GREEN, 1, "Connected to InfluxDB,  Nb data send : ");
        M5.Lcd.print(rec_count);
    }
    else
    {
        lcd_print(0, 200, RED, 1, "InfluxDB connection failed");
    }

    // Print the date and time
    time_t tnow = time(nullptr);
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(localtime(&tnow));

    // Shown starting time
    unsigned long t = (millis()); // milsec
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(170, 210);
    M5.Lcd.print("start : ");
    M5.Lcd.print(t / 86400000);
    M5.Lcd.print("J ");
    M5.Lcd.print((t % 86400000) / 3600000);
    M5.Lcd.print("H ");
    M5.Lcd.print((t % 3600000) / 60000);
    M5.Lcd.print("M ");
    M5.Lcd.print((t % 60000) / 1000);
    M5.Lcd.print("S");

    // Show wifi status
    if (data_value[15] == 1)
    {
        lcd_print(0, 210, GREEN, 2, "WIFI - OK");
    }
    else
    {
        lcd_print(0, 210, RED, 2, "WIFI - LOST");
    }
    lcd_print(170, 220, WHITE, 1, "WIFI signal: ");
    M5.Lcd.print(data_value[17]);
    M5.Lcd.print(" dBm");
    // print the button menu
    lcd_print(50, 230, BLUE, 1, "<=");
    lcd_print(150, 230, BLUE, 1, "HOME");
    lcd_print(260, 230, BLUE, 1, "=>");
    // Print the menu
    lcd_menu_print();
}