	//---------------PIN: PA3----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledA.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_ledA.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledA, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledA);

		//---------------PIN: PA10----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledB.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_ledB.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledB, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledB);

		//---------------PIN: PC1----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledC.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledC.GPIO_PinConfig.GPIO_PinNumber = PIN_1; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledC, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledC);

		//---------------PIN: PC2----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledD.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledD.GPIO_PinConfig.GPIO_PinNumber = PIN_2; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledD, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledD);

		//---------------PIN: PC3----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledE.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledE.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledE, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledE);

		//---------------PIN: PC4----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledF.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledF.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledF, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledF);

		//---------------PIN: PA2----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledG.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_ledG.GPIO_PinConfig.GPIO_PinNumber = PIN_2; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledG, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledG);

		//Definicion de los pines para la respectiva conexion de los catodos comunes

		//---------------PIN: PB3----------------
		//Definimos el periferico GPIOx a usar.
		handler_commonD.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_commonD.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_commonD, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_commonD);

		//---------------PIN: PB4----------------
		//Definimos el periferico GPIOx a usar.
		handler_commonU.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_commonU.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_commonU, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_commonU);


		//---------------TIM3------------------

		//Definimos un elemento del tipo GPIO_Handler_t (Struct)
		BasicTimer_Handler_t handlermultiplexacion = {0};
		//Definimos el timer a usar
		handlermultiplexacion.ptrTIMx = TIM3;
		//Definimos la configuracion del TIMER seleccionado
		handlermultiplexacion.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
		handlermultiplexacion.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
		handlermultiplexacion.TIMx_Config.TIMX_period = 1000;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
		//Cargamos la configuracion del TIMER especifico
		BasicTimer_Config(&handlermultiplexacion);

		//---------------TIM2------------------

		//Definimos el timer a usar
		handlerUserTimer.ptrTIMx = TIM2;
		//Definimos la configuracion del TIMER seleccionado
		handlerUserTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
		handlerUserTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
		handlerUserTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
		//Cargamos la configuracion del TIMER especifico
		BasicTimer_Config(&handlerUserTimer);

		//---------------TIM3------------------

		//Definimos el timer a usar
		handlermultiplexacion.ptrTIMx = TIM3;
		//Definimos la configuracion del TIMER seleccionado
		handlermultiplexacion.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
		handlermultiplexacion.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
		handlermultiplexacion.TIMx_Config.TIMX_period = 1000;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
		//Cargamos la configuracion del TIMER especifico
		BasicTimer_Config(&handlermultiplexacion);


