; ModuleID = 'Examples/test.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.4"

@safe_deref.arr = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3], align 4
@main.arr = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3], align 4

define i32 @wild_deref(i32* %ptr, i32 %off) nounwind uwtable ssp {
  %1 = alloca i32*, align 8
  %2 = alloca i32, align 4
  store i32* %ptr, i32** %1, align 8
  call void @llvm.dbg.declare(metadata !{i32** %1}, metadata !27), !dbg !29
  store i32 %off, i32* %2, align 4
  call void @llvm.dbg.declare(metadata !{i32* %2}, metadata !30), !dbg !31
  %3 = load i32* %2, align 4, !dbg !32
  %4 = sext i32 %3 to i64, !dbg !32
  %5 = load i32** %1, align 8, !dbg !32
  %6 = getelementptr inbounds i32* %5, i64 %4, !dbg !32
  %7 = load i32* %6, !dbg !32
  ret i32 %7, !dbg !32
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define i32 @safe_deref() nounwind uwtable ssp {
  %arr = alloca [3 x i32], align 4
  call void @llvm.dbg.declare(metadata !{[3 x i32]* %arr}, metadata !34), !dbg !39
  %1 = bitcast [3 x i32]* %arr to i8*, !dbg !40
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast ([3 x i32]* @safe_deref.arr to i8*), i64 12, i32 4, i1 false), !dbg !40
  %2 = getelementptr inbounds [3 x i32]* %arr, i32 0, i64 2, !dbg !41
  %3 = load i32* %2, align 4, !dbg !41
  ret i32 %3, !dbg !41
}

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture, i64, i32, i1) nounwind

define i32 @wild_deref2(i32* %ptr) nounwind uwtable ssp {
  %1 = alloca i32*, align 8
  store i32* %ptr, i32** %1, align 8
  call void @llvm.dbg.declare(metadata !{i32** %1}, metadata !42), !dbg !43
  %2 = load i32** %1, align 8, !dbg !44
  %3 = load i32* %2, !dbg !44
  ret i32 %3, !dbg !44
}

define i32 @unmod_cast() nounwind uwtable ssp {
  %val = alloca i32, align 4
  %addr = alloca i32, align 4
  call void @llvm.dbg.declare(metadata !{i32* %val}, metadata !46), !dbg !48
  store i32 3, i32* %val, align 4, !dbg !49
  call void @llvm.dbg.declare(metadata !{i32* %addr}, metadata !50), !dbg !51
  %1 = ptrtoint i32* %val to i32, !dbg !52
  store i32 %1, i32* %addr, align 4, !dbg !52
  %2 = load i32* %addr, align 4, !dbg !53
  %3 = sext i32 %2 to i64, !dbg !53
  %4 = inttoptr i64 %3 to i32*, !dbg !53
  %5 = load i32* %4, !dbg !53
  ret i32 %5, !dbg !53
}

define i32 @mod_cast() nounwind uwtable ssp {
  %val = alloca i32, align 4
  %addr = alloca i32, align 4
  call void @llvm.dbg.declare(metadata !{i32* %val}, metadata !54), !dbg !56
  store i32 3, i32* %val, align 4, !dbg !57
  call void @llvm.dbg.declare(metadata !{i32* %addr}, metadata !58), !dbg !59
  %1 = ptrtoint i32* %val to i32, !dbg !60
  %2 = or i32 %1, 1, !dbg !60
  store i32 %2, i32* %addr, align 4, !dbg !60
  %3 = load i32* %addr, align 4, !dbg !61
  %4 = sext i32 %3 to i64, !dbg !61
  %5 = inttoptr i64 %4 to i32*, !dbg !61
  %6 = load i32* %5, !dbg !61
  ret i32 %6, !dbg !61
}

define i32 @main() nounwind uwtable ssp {
  %1 = alloca i32, align 4
  %arr = alloca [3 x i32], align 4
  store i32 0, i32* %1
  call void @llvm.dbg.declare(metadata !{[3 x i32]* %arr}, metadata !62), !dbg !64
  %2 = bitcast [3 x i32]* %arr to i8*, !dbg !65
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* bitcast ([3 x i32]* @main.arr to i8*), i64 12, i32 4, i1 false), !dbg !65
  %3 = getelementptr inbounds [3 x i32]* %arr, i32 0, i32 0, !dbg !66
  %4 = call i32 @wild_deref(i32* %3, i32 3), !dbg !66
  %5 = getelementptr inbounds [3 x i32]* %arr, i32 0, i64 0, !dbg !66
  store i32 %4, i32* %5, align 4, !dbg !66
  ret i32 0, !dbg !67
}

!llvm.dbg.cu = !{!0}

!0 = metadata !{i32 720913, i32 0, i32 12, metadata !"Examples/test.c", metadata !"/Users/cryofrzd/Documents/Thesis/Git", metadata !"clang version 3.0 (: http://llvm.org/svn/llvm-project/safecode/branches/release_30/tools/clang)", i1 true, i1 false, metadata !"", i32 0, metadata !1, metadata !1, metadata !3, metadata !1} ; [ DW_TAG_compile_unit ]
!1 = metadata !{metadata !2}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4}
!4 = metadata !{metadata !5, metadata !12, metadata !15, metadata !18, metadata !21, metadata !24}
!5 = metadata !{i32 720942, i32 0, metadata !6, metadata !"wild_deref", metadata !"wild_deref", metadata !"", metadata !6, i32 4, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 256, i1 false, i32 (i32*, i32)* @wild_deref, null, null, metadata !10} ; [ DW_TAG_subprogram ]
!6 = metadata !{i32 720937, metadata !"Examples/test.c", metadata !"/Users/cryofrzd/Documents/Thesis/Git", null} ; [ DW_TAG_file_type ]
!7 = metadata !{i32 720917, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i32 0, i32 0, i32 0, metadata !8, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!8 = metadata !{metadata !9}
!9 = metadata !{i32 720932, null, metadata !"int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!10 = metadata !{metadata !11}
!11 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!12 = metadata !{i32 720942, i32 0, metadata !6, metadata !"safe_deref", metadata !"safe_deref", metadata !"", metadata !6, i32 9, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 0, i1 false, i32 ()* @safe_deref, null, null, metadata !13} ; [ DW_TAG_subprogram ]
!13 = metadata !{metadata !14}
!14 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!15 = metadata !{i32 720942, i32 0, metadata !6, metadata !"wild_deref2", metadata !"wild_deref2", metadata !"", metadata !6, i32 15, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 256, i1 false, i32 (i32*)* @wild_deref2, null, null, metadata !16} ; [ DW_TAG_subprogram ]
!16 = metadata !{metadata !17}
!17 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!18 = metadata !{i32 720942, i32 0, metadata !6, metadata !"unmod_cast", metadata !"unmod_cast", metadata !"", metadata !6, i32 20, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 0, i1 false, i32 ()* @unmod_cast, null, null, metadata !19} ; [ DW_TAG_subprogram ]
!19 = metadata !{metadata !20}
!20 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!21 = metadata !{i32 720942, i32 0, metadata !6, metadata !"mod_cast", metadata !"mod_cast", metadata !"", metadata !6, i32 27, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 0, i1 false, i32 ()* @mod_cast, null, null, metadata !22} ; [ DW_TAG_subprogram ]
!22 = metadata !{metadata !23}
!23 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!24 = metadata !{i32 720942, i32 0, metadata !6, metadata !"main", metadata !"main", metadata !"", metadata !6, i32 34, metadata !7, i1 false, i1 true, i32 0, i32 0, i32 0, i32 0, i1 false, i32 ()* @main, null, null, metadata !25} ; [ DW_TAG_subprogram ]
!25 = metadata !{metadata !26}
!26 = metadata !{i32 720932}                      ; [ DW_TAG_base_type ]
!27 = metadata !{i32 721153, metadata !5, metadata !"ptr", metadata !6, i32 16777219, metadata !28, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!28 = metadata !{i32 720911, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !9} ; [ DW_TAG_pointer_type ]
!29 = metadata !{i32 3, i32 21, metadata !5, null}
!30 = metadata !{i32 721153, metadata !5, metadata !"off", metadata !6, i32 33554435, metadata !9, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!31 = metadata !{i32 3, i32 30, metadata !5, null}
!32 = metadata !{i32 5, i32 5, metadata !33, null}
!33 = metadata !{i32 720907, metadata !5, i32 4, i32 1, metadata !6, i32 0} ; [ DW_TAG_lexical_block ]
!34 = metadata !{i32 721152, metadata !35, metadata !"arr", metadata !6, i32 10, metadata !36, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!35 = metadata !{i32 720907, metadata !12, i32 9, i32 1, metadata !6, i32 1} ; [ DW_TAG_lexical_block ]
!36 = metadata !{i32 720897, null, metadata !"", null, i32 0, i64 96, i64 32, i32 0, i32 0, metadata !9, metadata !37, i32 0, i32 0} ; [ DW_TAG_array_type ]
!37 = metadata !{metadata !38}
!38 = metadata !{i32 720929, i64 0, i64 2}        ; [ DW_TAG_subrange_type ]
!39 = metadata !{i32 10, i32 9, metadata !35, null}
!40 = metadata !{i32 10, i32 26, metadata !35, null}
!41 = metadata !{i32 11, i32 5, metadata !35, null}
!42 = metadata !{i32 721153, metadata !15, metadata !"ptr", metadata !6, i32 16777230, metadata !28, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!43 = metadata !{i32 14, i32 22, metadata !15, null}
!44 = metadata !{i32 16, i32 5, metadata !45, null}
!45 = metadata !{i32 720907, metadata !15, i32 15, i32 1, metadata !6, i32 2} ; [ DW_TAG_lexical_block ]
!46 = metadata !{i32 721152, metadata !47, metadata !"val", metadata !6, i32 21, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!47 = metadata !{i32 720907, metadata !18, i32 20, i32 1, metadata !6, i32 3} ; [ DW_TAG_lexical_block ]
!48 = metadata !{i32 21, i32 9, metadata !47, null}
!49 = metadata !{i32 21, i32 16, metadata !47, null}
!50 = metadata !{i32 721152, metadata !47, metadata !"addr", metadata !6, i32 22, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!51 = metadata !{i32 22, i32 9, metadata !47, null}
!52 = metadata !{i32 22, i32 26, metadata !47, null}
!53 = metadata !{i32 23, i32 5, metadata !47, null}
!54 = metadata !{i32 721152, metadata !55, metadata !"val", metadata !6, i32 28, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!55 = metadata !{i32 720907, metadata !21, i32 27, i32 1, metadata !6, i32 4} ; [ DW_TAG_lexical_block ]
!56 = metadata !{i32 28, i32 9, metadata !55, null}
!57 = metadata !{i32 28, i32 16, metadata !55, null}
!58 = metadata !{i32 721152, metadata !55, metadata !"addr", metadata !6, i32 29, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!59 = metadata !{i32 29, i32 9, metadata !55, null}
!60 = metadata !{i32 29, i32 30, metadata !55, null}
!61 = metadata !{i32 30, i32 5, metadata !55, null}
!62 = metadata !{i32 721152, metadata !63, metadata !"arr", metadata !6, i32 35, metadata !36, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!63 = metadata !{i32 720907, metadata !24, i32 34, i32 1, metadata !6, i32 5} ; [ DW_TAG_lexical_block ]
!64 = metadata !{i32 35, i32 9, metadata !63, null}
!65 = metadata !{i32 35, i32 26, metadata !63, null}
!66 = metadata !{i32 36, i32 14, metadata !63, null}
!67 = metadata !{i32 37, i32 5, metadata !63, null}
