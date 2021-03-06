#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

static GFont s_time_font;
static GFont s_date_font;

static BitmapLayer *s_background_layer;
static BitmapLayer *s_time_format_layer;
static GBitmap *s_background_bitmap;
static GBitmap *s_time_format_bitmap = NULL;

uint8_t get_display_hour(uint8_t hour) {
  if(clock_is_24h_style()) {
    return hour;
  } else {
    uint8_t display_hour = hour % 12;
    return display_hour ? display_hour : 12;
  }
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *current_time = localtime(&temp);
	
  // Create a long-lived buffer
  static char timebuffer[] = "00:00";
  static char datebuffer[] = "00000000 00 0000";
	
  // Write the current hours and minutes into the buffer
  if(!clock_is_24h_style()) {
    gbitmap_destroy(s_time_format_bitmap);
    s_time_format_bitmap = NULL;
    if(current_time->tm_hour <= 3) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NBC);
    } else if (current_time->tm_hour == 4 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ETODAY);
    } else if (current_time->tm_hour == 4 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWSMORN);
    } else if (current_time->tm_hour == 5) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWSMORN);
    } else if (current_time->tm_hour == 6) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWSMORN);
    } else if (current_time->tm_hour == 12 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PM_MODE);
    } else if (current_time->tm_hour == 12 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NBC);
    } else if (current_time->tm_hour == 13 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WTC);
    } else if (current_time->tm_hour == 13 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BEARS);
    } else if (current_time->tm_hour == 14 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WTC);
    } else if (current_time->tm_hour == 14 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BEARS);
    } else if (current_time->tm_hour == 15) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NBC);
    } else if (current_time->tm_hour == 16) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWS4);
    } else if (current_time->tm_hour == 17 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWS5);
    } else if (current_time->tm_hour == 17 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NN);
    } else if (current_time->tm_hour == 18 && current_time->tm_min <= 29) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWS6);
    } else if (current_time->tm_hour == 18 && current_time->tm_min >= 30) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NBC);
    } else if (current_time->tm_hour == 19) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BEARS);
    } else if (current_time->tm_hour == 20) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WTC);
    } else if (current_time->tm_hour == 21) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWS9);
    } else if (current_time->tm_hour == 22 && current_time->tm_min <= 34) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9NEWS10);
    } else if (current_time->tm_hour == 22 && current_time->tm_min >= 35) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TTS);
    } else if (current_time->tm_hour == 23 && current_time->tm_min <= 34) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TTS);
    } else if (current_time->tm_hour == 23 && current_time->tm_min >= 35) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LNWSM);
    } else if (current_time->tm_hour >= 24) {
      s_time_format_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NBC);
    } else {
      layer_remove_from_parent(bitmap_layer_get_layer(s_time_format_layer));
      bitmap_layer_destroy(s_time_format_layer);
    }
    if(s_time_format_bitmap != NULL) {
      if(s_time_format_layer == NULL) {
        s_time_format_layer = bitmap_layer_create(GRect(0, 0, 144, 127));
        layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_time_format_layer));
      }
      bitmap_layer_set_bitmap(s_time_format_layer, s_time_format_bitmap);
      layer_mark_dirty(bitmap_layer_get_layer(s_time_format_layer));
    }
  }
  {
    strftime(timebuffer, sizeof("00:00"), "%l:%M", current_time);
    strftime(datebuffer, sizeof("00000000 00 0000"), "%B %e %G", current_time);
  }
  text_layer_set_text(s_time_layer, timebuffer); 	
  text_layer_set_text(s_date_layer, datebuffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_background_layer));
	
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(-37, 124, 144, 74));
#ifdef PBL_COLOR
  text_layer_set_background_color(s_time_layer, GColorClear);
#else
  text_layer_set_background_color(s_time_layer, GColorClear);
#endif
#ifdef PBL_COLOR
    text_layer_set_text_color(s_time_layer, GColorWhite);
#else
  text_layer_set_text_color(s_time_layer, GColorWhite);
#endif
  text_layer_set_text(s_time_layer, "00:00");
	
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(84, 123, 58, 168));
#ifdef PBL_COLOR
  text_layer_set_background_color(s_date_layer, GColorClear);
#else
  text_layer_set_background_color(s_date_layer, GColorClear);
#endif
#ifdef PBL_COLOR
    text_layer_set_text_color(s_date_layer, GColorBlack);
#else
  text_layer_set_text_color(s_date_layer, GColorBlack);
#endif
  text_layer_set_text(s_date_layer, "00000000 00 0000");

  text_layer_set_overflow_mode(s_date_layer, GTextOverflowModeWordWrap);
	
  //Create GFont
 s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_28));
	
  //Create GFont
  s_date_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
	
  //Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
  //Apply to TextLayer
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	

  if(!clock_is_24h_style()) {
    s_time_format_bitmap = NULL;
    s_time_format_layer = bitmap_layer_create(GRect(0, 0, 144, 127));
  }
  layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_time_format_layer));
  update_time();
}

static void main_window_unload(Window *window) {
	
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_time_font);	
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_background_bitmap);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);
}
  
static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}