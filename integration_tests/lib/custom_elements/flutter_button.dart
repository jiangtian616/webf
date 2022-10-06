import 'package:flutter/material.dart';
import 'package:webf/webf.dart';

class FlutterButtonElement extends WidgetElement {
  FlutterButtonElement(BindingContext? context) : super(context);

  handlePressed(BuildContext context) {
    dispatchEvent(Event(EVENT_CLICK));
  }

  Widget buildButton(BuildContext context, String type, Widget child) {
    switch (type) {
      case 'primary':
        return ElevatedButton(onPressed: () => handlePressed(context), child: child);
      case 'default':
      default:
        return OutlinedButton(onPressed: () => handlePressed(context), child: child);
    }
  }

  @override
  Widget build(BuildContext context, List<Widget> children) {
    String type = getAttribute('type') ?? 'default';
    print('left: ${renderStyle.width.computedValue}');
    return ConstrainedBox(
        constraints:
            BoxConstraints.tightFor(width: renderStyle.width.computedValue, height: renderStyle.height.computedValue),
        child: buildButton(context, type, children.isNotEmpty ? children[0] : Container()));
  }
}
