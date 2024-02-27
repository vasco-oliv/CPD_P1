subroutine OnMultBlock(m_ar, m_br, bkSize)
    integer, intent(in) :: m_ar, m_br, bkSize
    integer :: Time1, Time2, clock_rate, clock_max
    real*8, allocatable :: pha(:,:), phb(:,:), phc(:,:)
    real*8 :: temp, timing
    integer :: i, j, k, size, ii, jj, kk
    integer :: unit

    allocate(pha(m_ar, m_ar), phb(m_ar, m_ar), phc(m_ar, m_ar))

    do i = 1, m_ar
        do j = 1, m_ar
            pha(i, j) = 1.0d0
        end do
    end do

    do i = 1, m_br
        do j = 1, m_br
            phb(i, j) = dble(i)
        end do
    end do

    call SYSTEM_CLOCK(count=Time1, count_rate=clock_rate, count_max=clock_max)

    ! do ii = 1, m_ar, bkSize
    !     print *, "Passsed HEre!"
    !     do jj = 1, m_br, bkSize
    !         do kk = 1, m_ar, bkSize
    !             do i = ii, ii + bkSize
    !                 do j = jj, jj+bkSize
    !                     do k = kk, kk+bkSize
    !                         phc(i, j) = phc(i, j) + pha(i, k) * phb(k, j)
    !                     end do
    !                 end do
    !             end do
    !         end do
    !     end do
    ! end do


        do ii = 1, m_ar, bkSize
        do jj = 1, m_br, bkSize
            do kk = 1, m_ar, bkSize
                do i = ii, min(ii + bkSize - 1, m_ar)
                    do j = jj, min(jj + bkSize - 1, m_br)
                        do k = kk, min(kk + bkSize - 1, m_ar)
                            phc(i, j) = phc(i, j) + pha(i, k) * phb(k, j)
                        end do
                    end do
                end do
            end do
        end do
    end do
    call SYSTEM_CLOCK(count=Time2)

    timing = real(Time2 - Time1) / real(clock_rate)

    ! Open CSV file for appending
    open(unit=unit, file='single_mult_block_fortran.csv', status='old', action='write', position='append', iostat=ierr)
    if (ierr /= 0) then
        print *, "Error opening file!"
        stop
    end if

    ! Write size and timing to CSV file
    if (unit == 10) then
        write(unit, '(A)') "size, time"
    end if
    write(unit, '(I0, A, I0, A, F0.3)') m_ar, ",", bkSize, ",", timing

    ! print *, "Result matrix:"
    ! do i = 1, 1
    !     do j = 1, min(10, m_br)
    !         print *, phc(i,j)
    !     end do
    ! end do

    ! Close CSV file
    close(unit)

    deallocate(pha, phb, phc)
end subroutine OnMultBlock


program Main
    implicit none
    integer :: size, m_ar, m_br, i, bksize, j
    real*8 :: timing

    ! Initialize starting size
    size = 600

    ! Loop until size reaches 3000
    ! do while (size <= 3000)
    !     ! Set matrix dimensions
    !     m_ar = size
    !     m_br = size

    !     call OnMultBlock(m_ar, m_br)

    !     call OnMultBlock(m_ar, m_br)

    !     ! Increment size by 400
    !     size = size + 400
    ! end do

    bksize = 128

    size = 600

    ! Loop until size reaches 3000
    do while (size <= 3000)
        ! Set matrix dimensions
        m_ar = size
        m_br = size
        do while ( bkSize <= 512)
            call OnMultBlock(m_ar, m_br, bkSize)

            call OnMultBlock(m_ar, m_br, bkSize)
            bkSize = bkSize *2
        end do
        bkSize = 128
        ! Increment size by 400
        size = size + 400
    end do


end program Main
