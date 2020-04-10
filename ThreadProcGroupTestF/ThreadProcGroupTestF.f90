!  ThreadProcGroupTestF.f90 
!
!  FUNCTIONS:
!  ThreadProcGroupTestF - Entry point of console application.
!

!****************************************************************************
!
!  PROGRAM: ThreadProcGroupTestF
!
!  PURPOSE:  Entry point for the console application.
!
!****************************************************************************

    program ThreadProcGroupTestF
    USE OMP_LIB
    implicit none
    ! Variables
    INTEGER I,J,K, T_N, N_T
    REAL A,AA
 
    ! Body of ThreadProcGroupTestF
    print *, 'Hello World'
      
    print*,"STARTING TEST"

!$OMP PARALLEL PRIVATE(N_T, T_N)
    N_T = OMP_GET_NUM_THREADS()
    T_N = OMP_GET_THREAD_NUM()
!    CALL SETTHREADPROCESSORGROUP(T_N,N_T)
!$OMP END PARALLEL

    print*,"STARTING LOOP"
      
    AA = 0.0
!$OMP PARALLEL SHARED(AA) PRIVATE(A)
    A = 0
!$OMP DO
    DO I = 1, 1000000
        DO J = 1, 1000000
            DO K = 1, 1000
                A = A + J + I + 10 * J + K
            ENDDO
        ENDDO
    ENDDO
!$OMP ENDDO
!$OMP CRITICAL
    AA = AA + A
!$OMP END CRITICAL
!$OMP END PARALLEL

    print*,"COMPLETE"
    end program ThreadProcGroupTestF

