scan-level:
для текущего количества измерений усреднения, размера вертикального шага и угла между замерами шагнуть вверх и сделать замеры по полному обороту.
если во время вертикального шага нажмется верхняя кнопка, выведется строка height_limit, остановится команда.
если замер выдаст слишком большое расстояние, выведется строка object_limit, остановится команда.
в конце выведется строка значений полученных точек в форме троек координат:
<level_parts_count>:<x_1>,<y_1>,<z_1>;<x_2>,<y_2>,<z_2> ... <z_(level_parts_count)>;

set-speed <N>:
установить угол между замерами в N умножить на начальное значение и размер вертикального шага в N умножить на начальное значение.
если не получится считать число N, выведется cant_read_int.
если количество замеров на уровне должно стать больше размера буфера замеров уровня, выведется строка too_many_parts, остановится команда.

move-to-bottom:
спустить лидар до нажатия нижней кнопки.

initial-move-up <N>:
поднять лидар на N шагов над уровнем стола и сбросить высоту (третью координату).
если не передать N, оно возьмется как высота стола.

calibrate:
калибровка. надо поставить объект краем в центр стола и запустить калибровку. после этого можно scan-level.
если замер выдаст слишком большое расстояние, выведется строка cant_see_object, остановится команда.

resend-level:
отправить заново строку значений полученных при последнем scan-level.
если ни одного уровня не сканировали, выведется строка no_levels.

set-sample-size <N>:
установить количество измерений усреднения в N.
если не получится считать число N, выведется cant_read_int.

