#include "windowrule.h"
#include "window.h"

namespace KWin
{

bool WindowRule::matches(EffectWindow *w)
{
    if (!enabled) {
        return false;
    } else if (matchAllWindows) {
        return true;
    }

    bool matches = true;
    if (matchWindowsByClass) {
        bool matchesClass = windowClasses.contains(w->window()->resourceName())
            || windowClasses.contains(w->window()->resourceClass());
        matches = (matchesClass && windowClassWhitelist) || (!matchesClass && !windowClassWhitelist);
    }

    if (matchWindowsByType) {
        matches = matches && ((matchNormalWindows && w->isNormalWindow())
            || (matchDialogWindows && w->isDialog())
            || (matchUtilityWindows && w->isUtility())
            || (matchDockWindows && w->isDock())
            || (matchToolbarWindows && w->isToolbar())
            || (matchContextMenuWindows && (w->isMenu() || w->isDropdownMenu() || w->isPopupMenu() || w->isPopupWindow())));
    }

    return matches;
}

}