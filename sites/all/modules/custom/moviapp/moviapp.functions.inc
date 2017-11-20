<?php

function _add_moviapp($table,$fields){
  db_set_active('moviapp');
  $id = db_insert($table) 
  ->fields($fields)
  ->execute();

  db_set_active();

  return $id;

}

function _edit_moviapp($table,$fields,$condition) {
  db_set_active('moviapp');

  $id = db_update($table) 
  ->fields($fields)
  ->condition($condition['field'],$condition['value'],'=')
  ->execute();

  db_set_active();

  return $id;

}

function _get_by_id_moviapp($table,$condition){
  db_set_active('moviapp');

  $result = db_select($table,'t') 
  ->fields('t')
  ->condition($condition['field'],$condition['value'],'=')
  ->execute()
  ->fetchAssoc();

  db_set_active();

  return $result;

}



function _get_options_secretaria(){
  db_set_active('moviapp');

  $result = db_select('movcom_secretarias','t') 
  ->fields('t')
  ->execute()
  ->fetchAll();

  db_set_active();
  $options = array('Seleccionar');
  foreach ($result as $secretaria) {
    $options[$secretaria->secretaria_id] = $secretaria->secretaria_nombre;
  }

  //drupal_set_message('<pre>'.print_r($options,true).'</pre>');


  return $options;

}



function _get_all_moviapp($table){
  db_set_active('moviapp');

  $query = db_select($table,'t')
  ->fields('t');
  if ($table == 'movcom_entidades'){
     $query->addField('s','secretaria_nombre','secretaria');
     $query->leftJoin('movcom_secretarias','s','s.secretaria_id = t.secretaria_id');
  }
  $result = $query->execute()->fetchAll();

  db_set_active();


  return $result;

}
function _get_report_moviapp($type){

  db_set_active('moviapp');
  $report = array();
  $report[1]='SELECT u.usuario_cedula, u.usuario_nombre, u.usuario_celular, u.usuario_correo, e.entidad_nombre
FROM movcom_usuarios u, movcom_entidades e
WHERE u.entidad_id=e.entidad_id
ORDER BY e.entidad_nombre';
  $report[2]='SELECT v.vehiculo_placa, v.vehiculo_marca, v.vehiculo_modelo, v.vehiculo_color, v.vehiculo_cupo, u.usuario_nombre, e.entidad_nombre
FROM movcom_usuarios u, movcom_vehiculos v, movcom_entidades e
WHERE u.usuario_id=v.usuario_id AND u.entidad_id=e.entidad_id
ORDER BY e.entidad_nombre';
  $report[3]='SELECT u.usuario_nombre, e.entidad_nombre, v.vehiculo_placa, v.vehiculo_marca, v.vehiculo_modelo, v.vehiculo_color, v.vehiculo_cupo,
ruta_nombre, r.ruta_cupo, r.ruta_inicio_direccion, r.ruta_inicio_latitud, r.ruta_inicio_longitud,
r.ruta_destino_direccion, r.ruta_destino_latitud, r.ruta_destino_longitud
FROM movcom_usuarios u, movcom_vehiculos v, movcom_entidades e, movcom_rutas r
WHERE r.vehiculo_id=v.vehiculo_id AND v.usuario_id=u.usuario_id AND e.entidad_id=u.entidad_id
ORDER BY e.entidad_nombre';
  $report[4]='SELECT u.usuario_nombre, e.entidad_nombre, b.bicitrayecto_inicio_fecha, b.bicitrayecto_inicio_hora, b.bicitrayecto_inicio_latitud,
b.bicitrayecto_inicio_longitud, b.bicitrayecto_destino_fecha, b.bicitrayecto_destino_hora, b.bicitrayecto_destino_latitud,
b.bicitrayecto_destino_longitud,  b.bicitrayecto_metros
FROM movcom_usuarios u, movcom_entidades e, movcom_bicitrayectos b
WHERE b.usuario_id=u.usuario_id AND e.entidad_id=u.entidad_id
ORDER BY e.entidad_nombre';
  $report[5]='SELECT u.usuario_nombre, e.entidad_nombre,
s.sitio_nombre, s.sitio_descripcion, s.sitio_latitud, s.sitio_longitud, s.sitio_direccion
FROM movcom_usuarios u, movcom_entidades e, movcom_sitios_favoritos s
WHERE s.usuario_id=u.usuario_id AND e.entidad_id=u.entidad_id
ORDER BY e.entidad_nombre';

  $query = db_query($report[$type]);
  $result = $query->fetchAll();

  db_set_active();


  return $result;

}

