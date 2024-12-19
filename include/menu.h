#ifndef MENU_H__
#define MENU_H__

#ifdef __cplusplus
extern "C" {
#endif

void menu_core_run(void);
extern void menu_actions_execute(int category, int action_id);

#ifdef __cplusplus
}
#endif

#endif /* MENU_H__ */
