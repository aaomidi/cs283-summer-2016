package com.aaomidi.jchat.chat.handlers;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.util.CharsetUtil;

public class ChatServerHandler extends ChannelInboundHandlerAdapter {
    int msgCount = 0;

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.printf("New client?\r\n");

    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        ByteBuf message = (ByteBuf) msg;
        ByteBuf buf = ctx.alloc().buffer();
        buf.writeCharSequence(String.format("Hi Bb%d%n", msgCount++), CharsetUtil.UTF_8);

        System.out.print(message.toString(CharsetUtil.UTF_8));
        System.out.println();

        ctx.writeAndFlush(buf);

        message.release();
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        System.err.printf("An exception was caught!\n");
        cause.printStackTrace();
        ctx.close();
    }


}
