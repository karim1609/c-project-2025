# THEME MODULE - ALL FUNCTIONS EXPLANATION

This document explains all functions in the theme module of the student management system. This module handles GTK+ theming, color schemes, fonts, spacing, and visual appearance customization.

## Data Structures

### ThemeType (Enum)
Defines the type of theme:
- `THEME_LIGHT = 0`: Light color scheme
- `THEME_DARK = 1`: Dark color scheme  
- `THEME_AUTO = 2`: Automatically follows system theme

### ColorScheme
Structure containing all color definitions for a theme:
- `name[50]`: Name of the color scheme
- `background_color[20]`: Main background color (hex format)
- `foreground_color[20]`: Main text/foreground color
- `primary_color[20]`: Primary action color (buttons, links)
- `secondary_color[20]`: Secondary action color
- `accent_color[20]`: Accent/highlight color
- `error_color[20]`: Error message color (typically red)
- `warning_color[20]`: Warning message color (typically orange)
- `success_color[20]`: Success message color (typically green)
- `info_color[20]`: Information message color (typically blue)
- `border_color[20]`: Border/outline color
- `selection_color[20]`: Selected item background
- `hover_color[20]`: Hover state color
- `disabled_color[20]`: Disabled widget color

### ThemeConfig
Main theme configuration structure:
- `current_theme`: Active theme type (ThemeType)
- `light_colors`: Light mode color scheme
- `dark_colors`: Dark mode color scheme
- `font_family[50]`: Font family name (e.g., "Roboto", "Arial")
- `font_size`: Base font size in pixels
- `font_size_large`: Large text font size
- `font_size_small`: Small text font size
- `border_radius`: Corner rounding radius in pixels
- `padding_small/medium/large`: Padding values for spacing
- `margin_small/medium/large`: Margin values for spacing
- `animation_duration`: Animation duration in milliseconds
- `enable_animations`: Flag to enable/disable animations (1/0)
- `enable_transparency`: Flag to enable transparency (1/0)
- `transparency_level`: Transparency alpha value (0.0-1.0)

---

## 1. THEME CONFIGURATION MANAGEMENT

### `theme_config_create(void)`
**Purpose**: Creates and initializes a new ThemeConfig structure with default values
**Returns**: Pointer to newly allocated ThemeConfig (must be freed with `theme_config_destroy`)
**Details**: 
- Allocates memory for ThemeConfig
- Initializes all fields with default values
- Sets up default light and dark color schemes
- Loads default font settings

### `theme_config_destroy(ThemeConfig* config)`
**Purpose**: Frees memory allocated for ThemeConfig structure
**Parameters**: 
- `config`: Pointer to ThemeConfig to destroy
**Returns**: void
**Details**: 
- Frees all allocated memory
- Safe to call with NULL pointer
- Prevents memory leaks

### `theme_config_load(ThemeConfig* config, const char* config_file)`
**Purpose**: Loads theme configuration from a file
**Parameters**: 
- `config`: Pointer to ThemeConfig structure to populate
- `config_file`: Path to configuration file
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Reads theme settings from file
- Parses color schemes, fonts, spacing values
- Updates config structure with loaded values
- Creates default config if file doesn't exist

### `theme_config_save(ThemeConfig* config, const char* config_file)`
**Purpose**: Saves current theme configuration to a file
**Parameters**: 
- `config`: Pointer to ThemeConfig to save
- `config_file`: Path to save configuration file
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Writes all theme settings to file
- Preserves user customizations
- Creates backup before overwriting
- Human-readable format

### `theme_config_set_theme(ThemeConfig* config, ThemeType theme)`
**Purpose**: Sets the current active theme type
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `theme`: Theme type to set (THEME_LIGHT, THEME_DARK, THEME_AUTO)
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates current_theme field
- May trigger theme reload if AUTO is selected
- Validates theme type

---

## 2. THEME APPLICATION

### `theme_apply_theme(ThemeConfig* config, GtkApplication* app)`
**Purpose**: Applies theme configuration to entire GTK application
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `app`: GTK Application object
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies theme to all windows in application
- Loads CSS provider
- Updates color scheme globally
- May restart application if needed

### `theme_apply_to_window(GtkWindow* window, ThemeConfig* config)`
**Purpose**: Applies theme to a specific GTK window
**Parameters**: 
- `window`: GTK Window widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies theme to window and all child widgets
- Updates window background
- Applies CSS styling
- Recursively styles all widgets

### `theme_apply_to_widget(GtkWidget* widget, ThemeConfig* config)`
**Purpose**: Applies theme to a specific widget
**Parameters**: 
- `widget`: GTK Widget to style
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies colors, fonts, spacing to widget
- Updates widget CSS classes
- Handles widget-specific styling
- Recursively applies to child widgets

### `theme_apply_css(GtkWidget* widget, const char* css_content)`
**Purpose**: Applies custom CSS directly to a widget
**Parameters**: 
- `widget`: GTK Widget to style
- `css_content`: CSS string to apply
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Parses CSS content
- Applies via GTK CSS provider
- Overrides default styling
- Useful for custom styling

---

## 3. CSS GENERATION

### `theme_generate_css(ThemeConfig* config)`
**Purpose**: Generates complete CSS stylesheet from theme configuration
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to allocated CSS string (caller must free)
- `NULL` on error
**Details**: 
- Generates comprehensive CSS
- Includes all colors, fonts, spacing
- GTK3/GTK4 compatible
- Uses current theme's color scheme

### `theme_generate_button_css(ThemeConfig* config)`
**Purpose**: Generates CSS specifically for buttons
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for buttons (caller must free)
- `NULL` on error
**Details**: 
- Styles button appearance
- Includes hover, active, disabled states
- Uses primary/secondary colors
- Border radius and padding

### `theme_generate_entry_css(ThemeConfig* config)`
**Purpose**: Generates CSS for text entry widgets
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for entries (caller must free)
- `NULL` on error
**Details**: 
- Styles text input fields
- Border colors, focus states
- Background and text colors
- Placeholder styling

### `theme_generate_treeview_css(ThemeConfig* config)`
**Purpose**: Generates CSS for tree/list view widgets
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for treeviews (caller must free)
- `NULL` on error
**Details**: 
- Styles table/tree views
- Row selection colors
- Header styling
- Alternate row colors

### `theme_generate_menu_css(ThemeConfig* config)`
**Purpose**: Generates CSS for menu widgets
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for menus (caller must free)
- `NULL` on error
**Details**: 
- Styles dropdown menus
- Menu item hover states
- Separator styling
- Submenu appearance

### `theme_generate_toolbar_css(ThemeConfig* config)`
**Purpose**: Generates CSS for toolbar widgets
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for toolbars (caller must free)
- `NULL` on error
**Details**: 
- Styles application toolbars
- Button spacing
- Background and borders
- Icon alignment

### `theme_generate_statusbar_css(ThemeConfig* config)`
**Purpose**: Generates CSS for status bar widgets
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for statusbars (caller must free)
- `NULL` on error
**Details**: 
- Styles bottom status bar
- Background color
- Text styling
- Separator styling

### `theme_generate_dialog_css(ThemeConfig* config)`
**Purpose**: Generates CSS for dialog windows
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to CSS string for dialogs (caller must free)
- `NULL` on error
**Details**: 
- Styles popup dialogs
- Window borders and shadows
- Button areas
- Content area styling

---

## 4. COLOR MANAGEMENT

### `theme_create_light_colors(void)`
**Purpose**: Creates a default light color scheme
**Returns**: Pointer to ColorScheme with light theme colors (caller must free)
**Details**: 
- Initializes all colors for light theme
- Uses light backgrounds, dark text
- Standard light theme palette
- High contrast for readability

### `theme_create_dark_colors(void)`
**Purpose**: Creates a default dark color scheme
**Returns**: Pointer to ColorScheme with dark theme colors (caller must free)
**Details**: 
- Initializes all colors for dark theme
- Uses dark backgrounds, light text
- Standard dark theme palette
- Reduced eye strain

### `theme_create_custom_colors(const char* name)`
**Purpose**: Creates a custom color scheme with specified name
**Parameters**: 
- `name`: Name for the color scheme
**Returns**: Pointer to ColorScheme (caller must free)
**Details**: 
- Creates empty color scheme
- Sets the name
- Colors must be set manually
- Useful for user customization

### `theme_set_color(ColorScheme* scheme, const char* color_name, const char* color_value)`
**Purpose**: Sets a specific color in a color scheme
**Parameters**: 
- `scheme`: Pointer to ColorScheme
- `color_name`: Name of color to set (e.g., "background_color", "primary_color")
- `color_value`: Hex color value (e.g., "#FF0000")
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates specified color in scheme
- Validates color format
- Supports all ColorScheme fields
- Case-insensitive color name matching

### `theme_get_color(ColorScheme* scheme, const char* color_name)`
**Purpose**: Retrieves a specific color value from color scheme
**Parameters**: 
- `scheme`: Pointer to ColorScheme
- `color_name`: Name of color to get
**Returns**: 
- Pointer to color string (internal storage, don't free)
- `NULL` if color not found
**Details**: 
- Returns hex color value
- Uses internal string storage
- Useful for querying current colors

### `theme_validate_color(const char* color)`
**Purpose**: Validates if a color string is in valid format
**Parameters**: 
- `color`: Color string to validate (hex format expected)
**Returns**: 
- `1` if valid
- `0` if invalid
**Details**: 
- Checks hex format (#RRGGBB or #RRGGBBAA)
- Validates hex digits
- May check color ranges
- Useful before setting colors

---

## 5. FONT MANAGEMENT

### `theme_set_font_family(ThemeConfig* config, const char* font_family)`
**Purpose**: Sets the font family for the theme
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `font_family`: Font name (e.g., "Roboto", "Arial", "Sans")
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates font_family field
- May validate font availability
- Applies to all text in theme
- System font fallback if font not found

### `theme_set_font_size(ThemeConfig* config, int font_size)`
**Purpose**: Sets the base font size
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `font_size`: Font size in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates font_size field
- Typically 10-16 pixels
- Affects most text
- Validates reasonable range

### `theme_set_font_size_large(ThemeConfig* config, int font_size)`
**Purpose**: Sets the large font size for headings/titles
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `font_size`: Large font size in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates font_size_large field
- Used for headings, titles
- Usually 1.2-1.5x base size
- Validates size > base size

### `theme_set_font_size_small(ThemeConfig* config, int font_size)`
**Purpose**: Sets the small font size for fine print
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `font_size`: Small font size in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates font_size_small field
- Used for captions, hints
- Usually 0.8-0.9x base size
- Validates size < base size

### `theme_get_font_family(ThemeConfig* config)`
**Purpose**: Gets current font family
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Pointer to font family string (internal storage)
**Details**: 
- Returns current font_family
- Never returns NULL
- Read-only access

### `theme_get_font_size(ThemeConfig* config)`
**Purpose**: Gets current base font size
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Font size in pixels
**Details**: 
- Returns font_size value
- Integer value

---

## 6. SPACING MANAGEMENT

### `theme_set_border_radius(ThemeConfig* config, int radius)`
**Purpose**: Sets the border radius for rounded corners
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `radius`: Border radius in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates border_radius field
- Affects button, entry corners
- 0 = square, higher = more rounded
- Typical range: 0-12 pixels

### `theme_set_padding(ThemeConfig* config, int small, int medium, int large)`
**Purpose**: Sets padding values for different sizes
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `small`: Small padding in pixels
- `medium`: Medium padding in pixels
- `large`: Large padding in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates all three padding values
- Internal spacing within widgets
- small < medium < large expected

### `theme_set_margin(ThemeConfig* config, int small, int medium, int large)`
**Purpose**: Sets margin values for different sizes
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `small`: Small margin in pixels
- `medium`: Medium margin in pixels
- `large`: Large margin in pixels
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates all three margin values
- External spacing between widgets
- small < medium < large expected

### `theme_get_border_radius(ThemeConfig* config)`
**Purpose**: Gets current border radius
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Border radius in pixels
**Details**: 
- Returns border_radius value

### `theme_get_padding(ThemeConfig* config, int* small, int* medium, int* large)`
**Purpose**: Gets all padding values
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `small`: Output parameter for small padding
- `medium`: Output parameter for medium padding
- `large`: Output parameter for large padding
**Returns**: void
**Details**: 
- Populates output parameters
- All three values retrieved at once

### `theme_get_margin(ThemeConfig* config, int* small, int* medium, int* large)`
**Purpose**: Gets all margin values
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `small`: Output parameter for small margin
- `medium`: Output parameter for medium margin
- `large`: Output parameter for large margin
**Returns**: void
**Details**: 
- Populates output parameters
- All three values retrieved at once

---

## 7. ANIMATION MANAGEMENT

### `theme_set_animation_duration(ThemeConfig* config, int duration)`
**Purpose**: Sets animation duration in milliseconds
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `duration`: Duration in milliseconds
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates animation_duration field
- Affects transition speeds
- Typical range: 100-500ms
- 0 = instant (no animation)

### `theme_enable_animations(ThemeConfig* config, int enable)`
**Purpose**: Enables or disables animations
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `enable`: 1 to enable, 0 to disable
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates enable_animations flag
- Disabling improves performance
- Accessibility consideration

### `theme_enable_transparency(ThemeConfig* config, int enable)`
**Purpose**: Enables or disables transparency effects
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `enable`: 1 to enable, 0 to disable
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates enable_transparency flag
- May affect window backgrounds
- Performance consideration

### `theme_set_transparency_level(ThemeConfig* config, float level)`
**Purpose**: Sets transparency level (alpha)
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `level`: Transparency level (0.0 = transparent, 1.0 = opaque)
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Updates transparency_level field
- Validates range 0.0-1.0
- Affects window/widget transparency

### `theme_get_animation_duration(ThemeConfig* config)`
**Purpose**: Gets current animation duration
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Duration in milliseconds
**Details**: 
- Returns animation_duration value

### `theme_are_animations_enabled(ThemeConfig* config)`
**Purpose**: Checks if animations are enabled
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` if enabled
- `0` if disabled
**Details**: 
- Returns enable_animations flag value

### `theme_is_transparency_enabled(ThemeConfig* config)`
**Purpose**: Checks if transparency is enabled
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` if enabled
- `0` if disabled
**Details**: 
- Returns enable_transparency flag value

### `theme_get_transparency_level(ThemeConfig* config)`
**Purpose**: Gets current transparency level
**Parameters**: 
- `config`: Pointer to ThemeConfig
**Returns**: 
- Transparency level (0.0-1.0)
**Details**: 
- Returns transparency_level value

---

## 8. THEME SWITCHING

### `theme_switch_to_light(ThemeConfig* config, GtkApplication* app)`
**Purpose**: Switches application to light theme
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `app`: GTK Application object
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Sets theme to THEME_LIGHT
- Applies light color scheme
- Updates all application windows
- May save preference

### `theme_switch_to_dark(ThemeConfig* config, GtkApplication* app)`
**Purpose**: Switches application to dark theme
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `app`: GTK Application object
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Sets theme to THEME_DARK
- Applies dark color scheme
- Updates all application windows
- May save preference

### `theme_switch_to_auto(ThemeConfig* config, GtkApplication* app)`
**Purpose**: Switches to auto theme (follows system)
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `app`: GTK Application object
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Sets theme to THEME_AUTO
- Detects system theme
- Applies matching theme
- Registers system theme change listener

### `theme_detect_system_theme(void)`
**Purpose**: Detects system-wide theme preference
**Returns**: 
- `THEME_LIGHT` if system uses light theme
- `THEME_DARK` if system uses dark theme
**Details**: 
- Queries system settings (GTK settings, OS preferences)
- Useful for AUTO theme mode
- Cross-platform detection

### `theme_on_system_theme_changed(ThemeConfig* config, GtkApplication* app)`
**Purpose**: Callback when system theme changes
**Parameters**: 
- `config`: Pointer to ThemeConfig
- `app`: GTK Application object
**Returns**: void
**Details**: 
- Called automatically when system theme changes
- Only active if THEME_AUTO is set
- Switches application theme to match system
- Updates all windows

---

## 9. CSS FILE OPERATIONS

### `theme_load_css_from_file(const char* filename, char** css_content)`
**Purpose**: Loads CSS content from a file
**Parameters**: 
- `filename`: Path to CSS file
- `css_content`: Output parameter - pointer to allocated CSS string
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Reads CSS file contents
- Allocates memory (caller must free)
- Returns entire file as string
- Validates file exists

### `theme_save_css_to_file(const char* filename, const char* css_content)`
**Purpose**: Saves CSS content to a file
**Parameters**: 
- `filename`: Path to save CSS file
- `css_content`: CSS string to save
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Writes CSS to file
- Creates file if doesn't exist
- Creates directory if needed
- Useful for exporting themes

### `theme_create_default_css_files(const char* css_dir)`
**Purpose**: Creates default CSS files for light and dark themes
**Parameters**: 
- `css_dir`: Directory to create CSS files in
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Creates light.css and dark.css
- Generates from default configurations
- Creates directory if needed
- Initial setup function

---

## 10. WIDGET-SPECIFIC THEMING

### `theme_style_button(GtkButton* button, ThemeConfig* config)`
**Purpose**: Applies theme styling to a button widget
**Parameters**: 
- `button`: GTK Button widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies colors, fonts, spacing
- Sets border radius
- Applies CSS classes
- Updates hover states

### `theme_style_entry(GtkEntry* entry, ThemeConfig* config)`
**Purpose**: Applies theme styling to an entry widget
**Parameters**: 
- `entry`: GTK Entry widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles text input fields
- Border colors and focus states
- Background and text colors
- Placeholder text styling

### `theme_style_treeview(GtkTreeView* treeview, ThemeConfig* config)`
**Purpose**: Applies theme styling to a treeview widget
**Parameters**: 
- `treeview`: GTK TreeView widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles list/table views
- Row selection colors
- Header styling
- Alternate row backgrounds

### `theme_style_menu(GtkMenu* menu, ThemeConfig* config)`
**Purpose**: Applies theme styling to a menu widget
**Parameters**: 
- `menu`: GTK Menu widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles dropdown menus
- Menu item appearance
- Hover and selection states
- Separator styling

### `theme_style_toolbar(GtkToolbar* toolbar, ThemeConfig* config)`
**Purpose**: Applies theme styling to a toolbar widget
**Parameters**: 
- `toolbar`: GTK Toolbar widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles application toolbar
- Button spacing and alignment
- Background colors
- Border styling

### `theme_style_statusbar(GtkStatusbar* statusbar, ThemeConfig* config)`
**Purpose**: Applies theme styling to a statusbar widget
**Parameters**: 
- `statusbar`: GTK Statusbar widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles bottom status bar
- Background color
- Text color and font
- Separator styling

### `theme_style_dialog(GtkDialog* dialog, ThemeConfig* config)`
**Purpose**: Applies theme styling to a dialog widget
**Parameters**: 
- `dialog`: GTK Dialog widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles popup dialogs
- Window appearance
- Button area styling
- Content area background

### `theme_style_window(GtkWindow* window, ThemeConfig* config)`
**Purpose**: Applies theme styling to a window widget
**Parameters**: 
- `window`: GTK Window widget
- `config`: Pointer to ThemeConfig
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Styles main window
- Background colors
- Title bar appearance
- Window decorations

---

## 11. COLOR UTILITIES

### `theme_hex_to_rgb(const char* hex, int* r, int* g, int* b)`
**Purpose**: Converts hex color string to RGB values
**Parameters**: 
- `hex`: Hex color string (e.g., "#FF0000")
- `r`: Output parameter for red component (0-255)
- `g`: Output parameter for green component (0-255)
- `b`: Output parameter for blue component (0-255)
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Parses hex color format
- Converts to RGB integers
- Validates hex format
- Useful for color calculations

### `theme_rgb_to_hex(int r, int g, int b)`
**Purpose**: Converts RGB values to hex color string
**Parameters**: 
- `r`: Red component (0-255)
- `g`: Green component (0-255)
- `b`: Blue component (0-255)
**Returns**: 
- Pointer to allocated hex string (caller must free)
- `NULL` on error
**Details**: 
- Formats RGB as "#RRGGBB"
- Allocates memory for string
- Validates RGB ranges
- Useful for color manipulation

### `theme_color_is_dark(const char* color)`
**Purpose**: Determines if a color is dark
**Parameters**: 
- `color`: Hex color string
**Returns**: 
- `1` if color is dark
- `0` if color is light
**Details**: 
- Calculates luminance
- Uses threshold to determine darkness
- Useful for choosing text color
- Helps with contrast

### `theme_color_is_light(const char* color)`
**Purpose**: Determines if a color is light
**Parameters**: 
- `color`: Hex color string
**Returns**: 
- `1` if color is light
- `0` if color is dark
**Details**: 
- Inverse of theme_color_is_dark
- Calculates luminance
- Useful for contrast calculations

### `theme_get_contrast_color(const char* background_color)`
**Purpose**: Gets a contrasting color for given background
**Parameters**: 
- `background_color`: Background color hex string
**Returns**: 
- Pointer to allocated contrast color hex string (caller must free)
- Returns white or black based on background
**Details**: 
- Calculates readable contrast color
- Returns white for dark backgrounds
- Returns black for light backgrounds
- Ensures text readability

### `theme_adjust_color_brightness(const char* color, float factor)`
**Purpose**: Adjusts color brightness
**Parameters**: 
- `color`: Original color hex string
- `factor`: Brightness factor (>1.0 = brighter, <1.0 = darker)
**Returns**: 
- Pointer to allocated adjusted color hex string (caller must free)
- `NULL` on error
**Details**: 
- Multiplies RGB values by factor
- Clamps values to valid range
- Useful for hover states
- Creates color variations

---

## 12. THEME PRESETS

### `theme_load_preset(ThemeConfig* config, const char* preset_name)`
**Purpose**: Loads a saved theme preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to populate
- `preset_name`: Name of preset to load
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Loads preset from file
- Updates config with preset values
- Validates preset format
- Useful for quick theme switching

### `theme_save_preset(ThemeConfig* config, const char* preset_name)`
**Purpose**: Saves current theme as a preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to save
- `preset_name`: Name for the preset
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Saves theme to preset file
- Creates preset directory if needed
- Overwrites existing preset with same name
- Allows user theme sharing

### `theme_list_presets(char*** preset_names, int* count)`
**Purpose**: Lists all available theme presets
**Parameters**: 
- `preset_names`: Output parameter - array of preset name strings
- `count`: Output parameter - number of presets found
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Scans preset directory
- Allocates array of strings (caller must free)
- Returns all preset names
- Useful for preset selection UI

### `theme_delete_preset(const char* preset_name)`
**Purpose**: Deletes a saved theme preset
**Parameters**: 
- `preset_name`: Name of preset to delete
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Removes preset file
- Cannot delete built-in presets
- Returns error if preset not found
- Permanent deletion

---

## 13. DEFAULT PRESETS

### `theme_load_default_light_preset(ThemeConfig* config)`
**Purpose**: Loads default light theme preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to populate
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies standard light theme
- White/light gray backgrounds
- Dark text colors
- Professional appearance

### `theme_load_default_dark_preset(ThemeConfig* config)`
**Purpose**: Loads default dark theme preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to populate
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Applies standard dark theme
- Dark gray/black backgrounds
- Light text colors
- Reduced eye strain

### `theme_load_high_contrast_preset(ThemeConfig* config)`
**Purpose**: Loads high contrast accessibility preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to populate
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Maximum contrast colors
- Accessibility focused
- Easier to read
- Black and white theme

### `theme_load_minimal_preset(ThemeConfig* config)`
**Purpose**: Loads minimal/clean preset
**Parameters**: 
- `config`: Pointer to ThemeConfig to populate
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Simple, clean design
- Minimal colors
- Reduced visual noise
- Focus on content

---

## 14. THEME VALIDATION

### `theme_validate_config(ThemeConfig* config)`
**Purpose**: Validates entire theme configuration
**Parameters**: 
- `config`: Pointer to ThemeConfig to validate
**Returns**: 
- `1` if valid
- `0` if invalid
**Details**: 
- Checks all fields for valid ranges
- Validates color schemes
- Checks font settings
- Ensures consistency

### `theme_validate_color_scheme(ColorScheme* scheme)`
**Purpose**: Validates a color scheme
**Parameters**: 
- `scheme`: Pointer to ColorScheme to validate
**Returns**: 
- `1` if valid
- `0` if invalid
**Details**: 
- Validates all color formats
- Checks for required colors
- Ensures contrast ratios
- Validates color names

### `theme_validate_css(const char* css_content)`
**Purpose**: Validates CSS syntax
**Parameters**: 
- `css_content`: CSS string to validate
**Returns**: 
- `1` if valid CSS
- `0` if invalid
**Details**: 
- Checks basic CSS syntax
- Validates selectors
- Checks property names
- May use CSS parser

---

## 15. THEME UTILITIES

### `theme_type_to_string(ThemeType theme)`
**Purpose**: Converts ThemeType enum to string
**Parameters**: 
- `theme`: ThemeType value
**Returns**: 
- Pointer to string representation
**Details**: 
- Returns "Light", "Dark", or "Auto"
- Constant strings, don't free
- Useful for display

### `theme_string_to_type(const char* theme_str)`
**Purpose**: Converts string to ThemeType enum
**Parameters**: 
- `theme_str`: Theme name string
**Returns**: 
- ThemeType enum value
**Details**: 
- Parses theme name
- Case-insensitive matching
- Returns THEME_LIGHT if unrecognized

### `theme_print_config(ThemeConfig* config)`
**Purpose**: Prints theme configuration to stdout (debugging)
**Parameters**: 
- `config`: Pointer to ThemeConfig to print
**Returns**: void
**Details**: 
- Human-readable output
- Shows all settings
- Useful for debugging
- Development tool

### `theme_print_color_scheme(ColorScheme* scheme)`
**Purpose**: Prints color scheme to stdout (debugging)
**Parameters**: 
- `scheme`: Pointer to ColorScheme to print
**Returns**: void
**Details**: 
- Prints all colors
- Shows hex values
- Useful for debugging
- Development tool

---

## Usage Examples

### Basic Theme Setup:
```c
ThemeConfig* theme = theme_config_create();
theme_config_load(theme, "data/theme.conf");
theme_apply_theme(theme, app);
```

### Switching Themes:
```c
theme_switch_to_dark(theme, app);
theme_config_save(theme, "data/theme.conf");
```

### Custom Colors:
```c
ColorScheme* colors = theme_create_custom_colors("My Theme");
theme_set_color(colors, "primary_color", "#2196F3");
theme_set_color(colors, "background_color", "#FFFFFF");
```

### Widget Styling:
```c
GtkButton* button = gtk_button_new_with_label("Click");
theme_style_button(button, theme);
```

---

## Constants

### Default Values:
- `THEME_DEFAULT_FONT_FAMILY`: "Roboto"
- `THEME_DEFAULT_FONT_SIZE`: 12 pixels
- `THEME_DEFAULT_BORDER_RADIUS`: 4 pixels
- `THEME_DEFAULT_ANIMATION_DURATION`: 200ms

### Standard Colors:
- `THEME_COLOR_BLUE_PRIMARY`: "#2196F3"
- `THEME_COLOR_GREEN_SUCCESS`: "#4CAF50"
- `THEME_COLOR_RED_ERROR`: "#F44336"
- `THEME_COLOR_ORANGE_WARNING`: "#FF9800"

### File Paths:
- `THEME_CONFIG_FILE`: "data/theme.conf"
- `THEME_LIGHT_CSS_FILE`: "assets/css/light.css"
- `THEME_DARK_CSS_FILE`: "assets/css/dark.css"
- `THEME_PRESETS_DIR`: "data/themes/"

---

## Summary

The theme module provides comprehensive functionality for:
- ✅ Complete theme configuration management
- ✅ Light and dark theme support with auto-detection
- ✅ Custom color scheme creation and management
- ✅ Font and spacing customization
- ✅ Animation and transparency controls
- ✅ CSS generation for all widget types
- ✅ Widget-specific styling functions
- ✅ Theme presets and preset management
- ✅ Color utility functions for manipulation
- ✅ File-based theme persistence
- ✅ System theme integration

This module is essential for creating a modern, customizable, and accessible user interface in the GTK+ student management system application.

