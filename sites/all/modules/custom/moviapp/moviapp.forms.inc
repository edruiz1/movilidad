<?php
/**
 * @file
 */
module_load_include('inc', 'moviapp', 'moviapp.functions');
function moviapp_dominio_form($form, &$form_state, $type = NULL,$id = NULL) {

  $form = array();
  if (!is_null($id)){
      switch ($type) {
        case 'dominio':
            $dominio = _get_by_id_moviapp('movcom_dominios',array('field'=>'dominio_id','value'=>$id));
            $form_state['values']['dominio_nombre'] = $dominio['dominio_nombre'];
            $form_state['values']['dominio_estado'] = $dominio['dominio_estado'];
          break;
        case 'secretaria':
            $dominio = _get_by_id_moviapp('movcom_secretarias',array('field'=>'secretaria_id','value'=>$id));
            $form_state['values']['secretaria_nombre'] = $dominio['secretaria_nombre'];
            $form_state['values']['secretaria_descripcion'] = $dominio['secretaria_descripcion'];
          break;
        case 'entidad':
            $dominio = _get_by_id_moviapp('movcom_entidades',array('field'=>'entidad_id','value'=>$id));
            $form_state['values']['entidad_nombre'] = $dominio['entidad_nombre'];
            $form_state['values']['entidad_descripcion'] = $dominio['entidad_descripcion'];
            $form_state['values']['secretaria_id'] = $dominio['secretaria_id'];
          break;
      }
  }

  $form['id'] = array (
    '#type'=>'hidden',
    '#value' => $id
    );
  $form['tipo'] = array (
    '#type'=>'hidden',
    '#value' => $type
    );

  $form[$type] = array (
     '#type'        => 'fieldset',
     '#tree'        => true, 
  );


  $form[$type][$type.'_nombre'] = array (
     '#type'        => 'textfield',
     '#title'       => t('Nombre'),
     '#description' => t('Digite nombre del dominio ej: test.com'),
     '#required'    => true,
     '#default_value' => isset($form_state['values'][$type.'_nombre'])?$form_state['values'][$type.'_nombre']:'',
  );

  switch ($type) {
    case 'dominio':
        $form[$type][$type.'_estado'] = array (
           '#type'        => 'radios',
           '#title'       => t('Estado'),
           '#required'    => true,
           '#options'     => array(0=>'Desactivado',1=>'Activado'),
           '#default_value' => isset($form_state['values'][$type.'_estado'])?$form_state['values'][$type.'_estado']:1,
        );
      break;
    case 'secretaria':
    case 'entidad':
        $form[$type][$type.'_descripcion'] = array (
           '#type'        => 'textfield',
           '#title'       => t('Descripcion'),
           '#required'    => true,
           '#default_value' => isset($form_state['values'][$type.'_descripcion'])?$form_state['values'][$type.'_descripcion']:'',
        );
        if ($type == 'entidad'){
            $form[$type]['secretaria_id'] = array (
               '#type'        => 'select',
               '#title'       => t('Secretaria'),
               '#required'    => true,
               '#options'     => _get_options_secretaria(),
               '#default_value' => isset($form_state['values']['secretaria_id'])?$form_state['values']['secretaria_id']:0,
            );

        }
      break;
  }



  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Grabar'),
    );

  return $form;
}


function moviapp_dominio_form_submit($form, &$form_state) {
  drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
  $tipo   = $form_state['values']['tipo'];
  $values = $form_state['values'][$tipo];
  switch ($tipo) {
    case 'dominio':
       $table    = 'movcom_dominios';
       $field_id = 'dominio_id';
      break;
    case 'secretaria':
       $table = 'movcom_secretarias';
       $field_id = 'secretaria_id';
      break;
    case 'entidad':
       $table = 'movcom_entidades';
       $field_id = 'entidad_id';
      break;
    
  }
  if (isset($form_state['values']['id']) && 
      !empty($form_state['values']['id']) && 
      $form_state['values'] > 0){
      $id = _edit_moviapp($table,$values,array('field'=>$field_id,'value'=>$form_state['values']['id']));
  }
  else {
      $id = _add_moviapp($table,$values);  
  }
  
}
