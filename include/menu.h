#ifndef MENU_H__
#define MENU_H__

#ifdef __cplusplus
extern "C" {
#endif

void menu_core_run(void);
void menu_actions_execute(int category, int action_id);
void menu_display_show_main_menu(void);
void menu_display_message(const char *msg);
void menu_display_error(const char *err_msg);

#ifdef __cplusplus
}
#endif

#endif /* MENU_H__ */
