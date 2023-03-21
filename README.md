# PCCD
furbo.


// Comprobar el número actual de mensajes en la cola de mensajes
    struct msqid_ds msqid_ds;
    if (msgctl(msqid, IPC_STAT, &msqid_ds) == -1) {
        perror("msgctl");
        exit(1);
    }
    int num_msgs = msqid_ds.msg_qnum;

    // Verificar si el número máximo de mensajes ha sido alcanzado
    if (num_msgs >= MAX_MESSAGES) {
        printf("No se pueden enviar más mensajes en este momento. Espere a que se retiren algunos mensajes de la cola.\n");
        exit(1);
    }
